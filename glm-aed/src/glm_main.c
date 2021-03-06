/******************************************************************************
 *                                                                            *
 * glm_main.c                                                                 *
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
#include <stdlib.h>
#include <string.h>

#include "glm.h"

#include "glm_types.h"
#include "glm_globals.h"

#ifdef PLOTS
   #include <libplot.h>
   extern CLOGICAL do_plots, saveall;
   extern char *plots_nml_name;
#ifdef XPLOTS
   extern int xdisp;
#endif
#endif


extern char glm_nml_file[];
extern void run_model(void);

/******************************************************************************/
int main(int argc, char *argv[])
{
    char *nmlfile = NULL;
    int all_ok = 1, show_options = 0;

#ifdef PLOTS
    saveall = 0;
#ifdef XPLOTS
    xdisp = 0;
#endif
#endif

    argv++; argc--;
    while (argc > 0) {
        if ( strcmp(*argv, "--nml") == 0) {
            argv++; argc--;
            nmlfile = *argv;
        }
#ifdef PLOTS
#ifdef XPLOTS
        else if (strcmp(*argv, "--xdisp") == 0) {
            xdisp = 1;
            if ( argc > 1 && strncmp(argv[1], "--", 2) != 0 ) {
                argv++; argc--;
                plots_nml_name = *argv;
            }
        }
#endif
        else if (strcmp(*argv, "--saveall") == 0) {
            if ( saveall == 0) saveall = 1;
        }
        else if (strcmp(*argv, "--save-all-in-one") == 0)
            saveall = 2;
#endif
        else {
            if (strcmp(*argv, "--help") != 0)
                printf("Unknown flag %s\n", *argv);
            show_options = 1;
            all_ok = 0;
        }
        argc--; argv++;
    }

#ifdef PLOTS
# ifdef XPLOTS
    do_plots = xdisp || saveall;
# else
    do_plots = saveall;
# endif
#endif

    printf("       ------------------------------------------------\n");
    printf("       |  General Lake Model (GLM)   Version %s    |\n", GLM_VERSION);
    printf("       ------------------------------------------------\n");

    if ( show_options ) {
       printf("--help  : show this blurb\n");
       printf("--nml <nmlfile> : get parameters from nmlfile\n");
#ifdef PLOTS
#ifdef XPLOTS
       printf("--xdisp : display temp/salt and selected others in x-window\n");
       printf("--xdisp <plotsfile> : like --xdisp, but use <plotsfile> instead of plots.nml\n");
#endif
       printf("--saveall : save plots to png files\n");
       printf("--save-all-in-one : save all plots to png file\n");
#endif
    }
    else if ( all_ok ) {
        if ( nmlfile != NULL ) strncpy(glm_nml_file, nmlfile, 256);
        run_model();

        printf("------------------------------------------------\n");
        printf("              Run Complete\n");
    }

    exit(0);
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#if DEBUG
void crash_()
{
    int *x = (int*)1;
    *x = 0;
}
#endif
