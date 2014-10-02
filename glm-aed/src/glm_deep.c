/******************************************************************************
 *                                                                            *
 * glm_deep.c                                                                 *
 *                                                                            *
 * Developed by :                                                             *
 *     AquaticEcoDynamics (AED) Group                                         *
 *     School of Earth & Environment                                          *
 *     The University of Western Australia                                    *
 *                                                                            *
 *     http://aed.see.uwa.edu.au/                                             *
 *                                                                            *
 * Copyright 2013, 2014 -  The University of Western Australia                *
 *                                                                            *
 *  This file is part of GLM (General Lake Model)                             *
 *                                                                            *
 *  GLM is free software: you can redistribute it and/or modify               *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  GLM is distributed in the hope that it will be useful,                    *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                            *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "glm.h"

#define DEBUGDIFF 0

/******************************************************************************/
#include "glm_types.h"
#include "glm_globals.h"
#include "glm_deep.h"
#include "glm_mixu.h"
#include "glm_util.h"
#include "aed_time.h"

//#define dbgprt(...) fprintf(stderr, __VA_ARGS__)
#define dbgprt(...) /* __VA_ARGS__ */

/*============================================================================*/
// GLOBAL VARIABLES

static REALTYPE dissipation;
static REALTYPE H_sml;              //# thickness of the upper mixed layer
static REALTYPE H_sig;

extern REALTYPE coef_mix_hyp;

#if DEBUGDIFF
static int count = 0;
#endif

#define _Scalars(i,j)     Scalars[_IDX_2d(MaxLayers,Num_WQ_Vars+2,i,j)]

/*============================================================================*/
static void join_scalar_colums(REALTYPE *Scalars);
static void calculate_diffusion(REALTYPE RTimeStep,
                                REALTYPE *tot_diffusivity, REALTYPE *Scalars, int difidx);


void do_diffusion()
{
//CONSTANTS
    REALTYPE exchka=80.0;
    REALTYPE dens_tol=1e-5;

//LOCALS
    REALTYPE exchk2;

#ifndef _VISUAL_C_
    // The dumb compiler on windows doesn't like this so must malloc manually
    REALTYPE Scalars[MaxLayers*(Num_WQ_Vars+2)];
    REALTYPE tot_diffusivity[MaxLayers];
#else
    REALTYPE *Scalars;
    REALTYPE *tot_diffusivity;
#endif
    REALTYPE RTimeStep;
    REALTYPE dif_exp;
    REALTYPE NSquared;

    int i, j, iTop;

    int flag;

/*----------------------------------------------------------------------------*/
//BEGIN
    iTop = surfLayer-1;

    //# return if already mixed.
    //# a semi-useless test given it is checked for before calling
    if (iTop < 0) return ;

    exchk2 = -exchka;

    //# Set the local time step
    RTimeStep = noSecs;

#ifdef _VISUAL_C_
    Scalars = malloc(sizeof(REALTYPE) * MaxLayers*(Num_WQ_Vars+2));
    tot_diffusivity = malloc(sizeof(REALTYPE) * MaxLayers);
#endif

    //# Set up array of diffusable species
    memset(Scalars, 0, sizeof(Scalars));
    join_scalar_colums(Scalars);

    //# Now calculate the turbulent diffusivities

    flag = TRUE;
    //# Look for any density variation
    if (iTop > botmLayer) {
        i = botmLayer + 1;
        while (i <= iTop) {
            if (Lake[i].Density-Lake[i-1].Density > dens_tol) {
                flag = FALSE;
                break;
            }
            i++;
        }
    }

    //# Create buoyancy frequency distribution
    for (i = (botmLayer+2); i <= iTop-1; i++) {
        //# Eq XX in GLM manual
        NSquared = gprime(Lake[i+2].Density, Lake[i-2].Density) / (Lake[i+2].MeanHeight - Lake[i-2].MeanHeight);
        if (NSquared <= 1.E-6)
            NSquared = zero;

        if (NSquared == 0.0 || vel == 0.0 || WaveNumSquared < 0.0) {
            Lake[i].Epsilon = zero;
            continue;
        }
        if (NSquared < 1.E-6 && vel < 1.E-6 && WaveNumSquared < 1.E-6) {
            Lake[i].Epsilon = zero;
            continue;
        } else
            Lake[i].Epsilon = coef_mix_hyp * dissipation / (NSquared + 0.600 * WaveNumSquared * vel * vel);

        if (flag && i == iTop) continue;
        if (Lake[i].Height > XMoment1) continue;
        if (H_sig <= zero) {
             Lake[i].Epsilon = zero;
             continue;
        }
        //* Exponent for diffusivity equation
        dif_exp=(-one * sqr(Lake[surfLayer].Height-H_sml-Lake[i].Height))/H_sig;
        //* Dissipation (Eq. X GLM manual)
        if (dif_exp < exchk2) Lake[i].Epsilon = zero;
        else                   Lake[i].Epsilon *= (exp(dif_exp)+1.E-7);
    }

    if (iTop == botmLayer) Lake[iTop].Epsilon = zero;
    else                   Lake[iTop].Epsilon = Lake[iTop-1].Epsilon;

    // Set boundary epsilons
    Lake[surfLayer].Epsilon = Lake[iTop].Epsilon;
    Lake[1].Epsilon = Lake[2].Epsilon;
    Lake[0].Epsilon = Lake[1].Epsilon;

    // Special case for epsilon under ice cover
    if (ice) {
        for (i = 0; i < NumLayers; i++) {
            if (Lake[i].MeanHeight < 3.)
                Lake[i].Epsilon = mol_diffusivity[0]*5. * Lake[i].MeanHeight / 3.0;
            else if (Lake[surfLayer].Height - Lake[i].MeanHeight < 3.)
                Lake[i].Epsilon = mol_diffusivity[0]*5. * (Lake[surfLayer].Height - Lake[i].MeanHeight) / 3.0;
            else if (Lake[i].MeanHeight >= 3.0 && (Lake[surfLayer].Height - Lake[i].MeanHeight) >= 3.0)
                Lake[i].Epsilon = mol_diffusivity[0] * 5.0;
        }
    }

    //# Now we have the diffusivity, loop through & diffuse each scalar
    for (j = 0; j < NumDif; j++) {
        for (i = 0; i < NumLayers; i++)
            tot_diffusivity[i] = (Lake[i].Epsilon + mol_diffusivity[j]);

        calculate_diffusion(RTimeStep, tot_diffusivity, Scalars, j);
    }

    for (i = 0; i < NumLayers; i++) {
        //# Update T, S and WQ
        Lake[i].Temp = _Scalars(i,0);
        Lake[i].Salinity = _Scalars(i,1) / (Lake[i].Density+thsnd);
        for (j = 2; j < Num_WQ_Vars+2; j++)
            _WQ_Vars(j-2,i) = _Scalars(i,j);

        //# Calculate the new densities
        Lake[i].Density = calculate_density(Lake[i].Temp,Lake[i].Salinity);
    }

    //# Check for instabilities - call check_layer_stability to mix
    i = botmLayer + 1;
    while (i <= surfLayer) {
        if (Lake[i].Density >= Lake[i-1].Density) {
            check_layer_stability();
            break;
        }
        i++;
    }

    //# Special addition of heat diffusivity to Epsilon
    //   (for reference by inflow and outflow)
    for (i = 0; i < NumLayers; i++)
        Lake[i].Epsilon += mol_diffusivity[0];

#ifdef _VISUAL_C_
    free(Scalars); free(tot_diffusivity);
#endif
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/******************************************************************************
 * This routine calculates the diffusion of scalars between two layers        *
 ******************************************************************************/
static void calculate_diffusion(REALTYPE RTimeStep,
                                REALTYPE *tot_diffusivity, REALTYPE *Scalars, int difidx)
{
#define TOL 1.0E-8

    REALTYPE meanConc, deltaConc, dep, dz1, dz2, exfac, factor, HalfTS, totalMass;
    int iBot, iTop, ia, ib, ic, i, k;

/*----------------------------------------------------------------------------*/

    HalfTS = RTimeStep/two;
    iBot = botmLayer;
    iTop = surfLayer-1;

#if DEBUGDIFF
    count = count + 1;

    dbgprt( "======== calculate_diffusion ===============\n");
    dbgprt( "surfLayer = %d difidx = %d count = %d\n", surfLayer,difidx,count);
    for (i = 0; i < NumLayers; i++)
        dbgprt( "C_array] Scalars[%d,%d] = %lf\n",i,difidx, _Scalars(i,difidx));
    dbgprt( "--------------------------------------------------------------------\n");
#endif

    //# Simple explicit diffusion calculation, performs the calculation in two
    //  directions (ie. diffuse to upper and then lower layers)
    for (k = 1; k <= 2; k++) {
        if (k == 1) {
        	// going from lower to upper layers
            ia = iBot;
            ib = iTop;
            ic = 1;
        } else {
        	// going from upper to lower layers
            ia = iTop;
            ib = iBot;
            ic = -1;
        }

        for (i = ia; i < ib; i+=ic ) {
            if (fabs(_Scalars(i,difidx))   < 1E-20) _Scalars(i,difidx) = 0.;
            if (fabs(_Scalars(i+1,difidx)) < 1E-20) _Scalars(i+1,difidx) = 0.;

            if (fabs(_Scalars(i,difidx) - _Scalars(i+1,difidx)) > TOL) {
                totalMass = _Scalars(i,difidx) * Lake[i].LayerVol +
                            _Scalars(i+1,difidx) * Lake[i+1].LayerVol;

                dep = zero;
                if (i > botmLayer) dep = Lake[i-1].Height;

                dz1 = Lake[i].Height - dep;
                dz2 = Lake[i+1].Height - Lake[i].Height;

                meanConc = (_Scalars(i,difidx) * dz1 + _Scalars(i+1,difidx) * dz2) / (dz2+dz1);
                deltaConc = _Scalars(i,difidx) - _Scalars(i+1,difidx);
                factor = ((tot_diffusivity[i] + tot_diffusivity[i+1])/two) * pow((two/(dz2+dz1)),2)*HalfTS;

                if (factor > twenty)
                    exfac = zero;
                else
                    exfac = exp(-factor);

                // set adjustments to layer concs based on the diffusion factor, and conc gradient dC/dz
                _Scalars(i,difidx) = meanConc+exfac*dz2*deltaConc/(dz2+dz1);
                _Scalars(i+1,difidx) = meanConc-exfac*dz1*deltaConc/(dz2+dz1);

                // ensure mass conservation
                if ((Lake[i].LayerVol/dz1-Lake[i+1].LayerVol/dz2) > zero)
                    _Scalars(i,difidx) = (totalMass - _Scalars(i+1,difidx)*Lake[i+1].LayerVol)/Lake[i].LayerVol;
                else
                    _Scalars(i+1,difidx) = (totalMass - _Scalars(i,difidx)*Lake[i].LayerVol)/Lake[i+1].LayerVol;
            }
        }
    }

#if DEBUGDIFF
    for (i = 0; i < NumLayers; i++)
       dbgprt( "C_array] Scalars[%d,%d] = %lf\n",i,difidx, _Scalars(i,difidx));
    dbgprt( "======== END calculate_diffusion ===============\n");
//exit(0);
#endif
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/******************************************************************************
 * This routine calculates the dissipation of TKE in response to the rate     *
 * of working and energy inputs by the wind & inflows                         *
 ******************************************************************************/
void do_dissipation()
{
    REALTYPE  pt5 = 0.5, areaFactor = 1.0E+6, volFactor = 1.0E+3, cwnsq1 = 12.4;

#ifndef _VISUAL_C_
    // The dumb compiler on windows doesn't like this so must malloc manually
    REALTYPE Nsquared[MaxLayers];
#else
    REALTYPE *Nsquared;
#endif
    REALTYPE CentreMassSML;
    REALTYPE CentreMassMIX;

    REALTYPE MixLayerVol;
    REALTYPE VTilda;
    REALTYPE XMoment0;
    REALTYPE MeanDensity;
    REALTYPE DFLOC;
    REALTYPE BuoySD;
    REALTYPE EINFW;
    REALTYPE EW;
    REALTYPE EWW;
    REALTYPE WindSpeedX;
    REALTYPE WindPower;
    REALTYPE uStar;
    REALTYPE dz_top;
    REALTYPE Sbig;
    REALTYPE Ssml;
    REALTYPE SMLOC;
    REALTYPE Sigma0M;
    REALTYPE Sigma2M;
    REALTYPE Tbig;
    REALTYPE Tsml;
    REALTYPE TMLOC;
    REALTYPE VMLOC;
    REALTYPE VMbig;
    REALTYPE VMsml;
    REALTYPE XZI;

    int i, kl;

/*----------------------------------------------------------------------------*/

#ifdef _VISUAL_C_
    Nsquared = malloc(sizeof(REALTYPE) * MaxLayers);
#endif

    //# Estimate rate of working by the wind (Patterson et al. 1977; p3)
    WindSpeedX = MetData.WindSpeed;
    WindPower = 1.9344 * 0.24 * WindSpeedX * WindSpeedX * WindSpeedX * 1.E-6;

    uStar = 0.0;
    if (!ice) uStar = sqrt(1.612E-6*pow(WindSpeedX, 2));

    //# Find the centre of buoyancy
    // "XMoment1" is 1st moment, "XMoment0" is 0th moment
    //  XMoment1 is the centre of buoyancy (in metres above bottom)
    memset(Nsquared, 0, sizeof(Nsquared));

    XMoment1 = zero;
    XMoment0 = zero;
    Lake[0].MeanHeight = Lake[0].Height/two;
    for (i = 1; i < NumLayers; i++) {
        Lake[i].MeanHeight = (Lake[i].Height+Lake[i-1].Height)/two;
        Nsquared[i] = gprime(Lake[i].Density,Lake[i-1].Density) /
                            (Lake[i].MeanHeight-Lake[i-1].MeanHeight);
        XMoment1 = XMoment1 + Lake[i-1].Height*Nsquared[i]*(Lake[i].LayerVol+Lake[i-1].LayerVol)/two;
        XMoment0 = XMoment0 + Nsquared[i]*(Lake[i].LayerVol+Lake[i-1].LayerVol)/two;
    }

     //# Define length scales: H_sml is the thickness of the surface mixed layer
     //   and dz_top is the thickness of the upper most layer layer
    if (XMoment1 != 0.) XMoment1=XMoment1/XMoment0;

    H_sml  = Lake[surfLayer].Height - XMoment1;
    dz_top = Lake[surfLayer].Height - Lake[surfLayer-1].Height;

    // CentreMassSML : centre of mass of surface mixed layer
    // CentreMassMIX : centre of mass of a entirely mixed water column
    CentreMassSML = zero;
    CentreMassMIX = zero;
    Tbig  = zero;
    Tsml  = zero;
    Sbig  = zero;
    Ssml  = zero;
    VMbig = zero;
    VMsml = zero;

    //# Calculate first moments
    for (i = 0; i < NumLayers; i++) {
        add_this_layer(&VMbig, &VMsml, &Tbig, &Tsml, &Sbig, &Ssml, &VMLOC, &TMLOC, &SMLOC, &DFLOC, i);
        CentreMassSML = CentreMassSML + Lake[i].MeanHeight * Lake[i].Density * Lake[i].LayerVol * volFactor;
        CentreMassMIX = CentreMassMIX + Lake[i].MeanHeight * Lake[i].LayerVol * volFactor;
    }

    //# find the layer with the centre of buoyancy
    i = botmLayer;
    while(1) {
        if ( (Lake[i].Height > XMoment1) || (i == surfLayer) ) break;
        i++;
    }

    /**************************************************************************
     * Calculate the variance of the buoyancy distribution about XMoment1     *
     *               Sigma0M = 0th moment of buoyancy about XMoment1            *
     *               Sigma2M = 2nd moment of buoyancy about XMoment1            *
     *               Sigma2M/Sigma0M = variance of buoyancy distribution          *
     *               BuoySD = standard deviation of buoyancy distribution     *
     **************************************************************************/
    Sigma0M = zero;
    Sigma2M = zero;
    H_sig = pow(XMoment1, two);
    if (i != botmLayer ) {
         for (kl = i; kl > botmLayer; kl--) {
            if (Nsquared[kl] <= zero) Nsquared[kl] = zero;
            XZI = Lake[i].MeanHeight-Lake[kl-1].MeanHeight;

            //# By using '2' for Sigma2M we assume a symmetrical distribution
            Sigma2M = Sigma2M + two * (pow(XZI, two)) * Nsquared[kl] * (Lake[kl].MeanHeight - Lake[kl-1].MeanHeight);
            Sigma0M = Sigma0M + Nsquared[kl] * (Lake[kl].MeanHeight - Lake[kl-1].MeanHeight);
        }
        if (Sigma0M > zero) H_sig = Sigma2M / Sigma0M;
        if (H_sig > pow(XMoment1, two)) H_sig = pow(XMoment1, two);
    }

    BuoySD = zero;
    if (H_sig > zero) BuoySD = pow(H_sig, pt5);

    //# Find the 1st layer above which 85% of N^2 lies
    //  and the volume that contains the 85% (VTilda)
    i = botmLayer;
    while(1) {
        if (Lake[i].Height > (XMoment1 - BuoySD)) break;
        if ( i == surfLayer-1 ) break;
        i++;
    }

    VTilda = Lake[surfLayer].Vol1;
    if (i > botmLayer) VTilda = (Lake[surfLayer].Vol1 - Lake[i-1].Vol1);

    //# Calculate rate of working by inflows
    //  einff is the rate of energy released by plunging
    //  i.e. the change in potential energy (see do_inflows)
    MixLayerVol = VTilda - Lake[surfLayer].LayerVol;
    MeanDensity = thsnd + (Lake[0].Density + Lake[surfLayer].Density) / two;
    EINFW = einff / (MixLayerVol*thsnd*MeanDensity);

    //# Include rate of working of the wind
    EW = WindPower * Lake[surfLayer].LayerArea * areaFactor;
    EWW = EW / (VTilda * thsnd * MeanDensity);

    //# Calculate dissipation, and associated velocity scale and wavenumber^2
    //  as required by the diffusivity calculation (done in do_diffusion)
   if ((EW + einff) > zero) {
        dissipation = EWW+EINFW;
        if (EWW > EINFW) {
            WaveNumSquared = cwnsq1*Lake[surfLayer].LayerArea*areaFactor/(VTilda*thsnd*dz_top);
            vel = uStar;
        }
    }
#ifdef _VISUAL_C_
    free(Nsquared);
#endif
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/******************************************************************************
 * This subroutine creates a single array of concentrations of the            *
 * scalars being diffused                                                     *
 *----------------------------------------------------------------------------*/
static void join_scalar_colums(REALTYPE *Scalars)
{
    int i,j;

    for (i = 0; i < NumLayers; i++) {
        _Scalars(i,0) = Lake[i].Temp;
        _Scalars(i,1) = Lake[i].Salinity*(Lake[i].Density+thsnd);

        for (j = 2; j < Num_WQ_Vars+2; j++)
            _Scalars(i,j) = _WQ_Vars(j-2,i);
    }
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*############################################################################*/
void check_layer_stability()
{
    int i,k,wqvidx;

     /*-----------------------------------------------------------------------*/

    while (surfLayer != botmLayer) {
        //# find an unstable layer configuration (instability)
        for (k = surfLayer; k >= (botmLayer+1); k--)
            if (Lake[k].Density > Lake[k-1].Density) break;

        if (k < (botmLayer+1)) return ;

        //# mix the unstable layer with the layer below
        //# do separately for T, S and WQ array
        Lake[k-1].Temp = combine(Lake[k].Temp,   Lake[k].LayerVol,   Lake[k].Density,
                                 Lake[k-1].Temp, Lake[k-1].LayerVol, Lake[k-1].Density);
        Lake[k-1].Salinity = combine(Lake[k].Salinity,   Lake[k].LayerVol,   Lake[k].Density,
                                     Lake[k-1].Salinity, Lake[k-1].LayerVol, Lake[k-1].Density);

        for (wqvidx = 0; wqvidx < Num_WQ_Vars; wqvidx++)
            _WQ_Vars(wqvidx,k-1) = combine_vol(_WQ_Vars(wqvidx,k),   Lake[k].LayerVol,
                                               _WQ_Vars(wqvidx,k-1), Lake[k-1].LayerVol);

        // now we need to update layer properties accordingly
        Lake[k-1].Height = Lake[k].Height;
        Lake[k-1].LayerVol = Lake[k-1].LayerVol + Lake[k].LayerVol;
        Lake[k-1].Vol1 = Lake[k].Vol1;
        Lake[k-1].LayerArea = Lake[k].LayerArea;
        Lake[k-1].MeanHeight = Lake[0].Height/two;
        if ((k-1) != botmLayer)
            Lake[k-1].MeanHeight = (Lake[k-1].Height + Lake[k-2].Height) / two;
        Lake[k-1].Density = calculate_density(Lake[k-1].Temp, Lake[k-1].Salinity);
        Lake[k-1].Epsilon = Lake[k].Epsilon;

        //# adjust layer numbering of layers above mixing include water quality and particles
        NumLayers--;
        for (i = k; i < NumLayers; i++) {
            Lake[i].Epsilon = Lake[i+1].Epsilon;
            Lake[i].Height = Lake[i+1].Height;
            Lake[i].MeanHeight = Lake[i+1].MeanHeight;
            Lake[i].Temp = Lake[i+1].Temp;
            Lake[i].Salinity = Lake[i+1].Salinity;

            for (wqvidx = 0; wqvidx < Num_WQ_Vars; wqvidx++)
                _WQ_Vars(wqvidx, i) = _WQ_Vars(wqvidx, i+1);

            Lake[i].LayerVol = Lake[i+1].LayerVol;
            Lake[i].Vol1 = Lake[i+1].Vol1;
            Lake[i].LayerArea = Lake[i+1].LayerArea;
            Lake[i].Density = Lake[i+1].Density;
        }
    }
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*============================================================================*/