#include "fabm_driver.h"
!-----------------------------------------------------------------------
!BOP
!
! !MODULE: examples_npzd_det - Fennel & Neumann 1996 NPZD model - detritus component
!
! !INTERFACE:
   module examples_npzd_det
!
! !DESCRIPTION:
! This model features a single detritus variable, characterized by a rate of decay (rdn)
! and a sinking rate. Mineralized detritus feeds into a dissolved mineral pool that must
! be provided by an external model (e.g., examples_npzd_nut).
!
! !USES:
   use fabm_types

   implicit none

   private
!
! !PUBLIC DERIVED TYPES:
   type,extends(type_base_model),public :: type_examples_npzd_det
!     Variable identifiers
      type (type_state_variable_id)     :: id_d
      type (type_state_variable_id)     :: id_mintarget

!     Model parameters
      real(rk) :: rdn

      contains

      procedure :: initialize
      procedure :: do
      procedure :: do_ppdd

   end type
!EOP
!-----------------------------------------------------------------------

   contains

!-----------------------------------------------------------------------
!BOP
!
! !IROUTINE: Initialise the Detritus model
!
! !INTERFACE:
   subroutine initialize(self,configunit)
!
! !DESCRIPTION:
!  Here, the examples_npzd_det namelist is read and variables exported
!  by the model are registered with FABM.
!
! !INPUT PARAMETERS:
   class (type_examples_npzd_det), intent(inout), target :: self
   integer,                        intent(in)            :: configunit
!
! !LOCAL VARIABLES:
   real(rk), parameter :: d_per_s = 1.0_rk/86400.0_rk
   real(rk)            :: w_d, kc
!EOP
!-----------------------------------------------------------------------
!BOC
   ! Store parameter values in our own derived type
   ! NB: all rates must be provided in values per day and are converted here to values per second.
   call self%get_parameter(w_d,     'w_d','m d-1',    'vertical velocity (<0 for sinking)',default=-5.0_rk,scale_factor=d_per_s)
   call self%get_parameter(kc,      'kc', 'm2 mmol-1','specific light extinction',         default=0.03_rk)
   call self%get_parameter(self%rdn,'rdn','d-1',      'remineralization rate',             default=0.003_rk,scale_factor=d_per_s)

   ! Register state variables
   call self%register_state_variable(self%id_d,'c','mmol/m**3','concentration',    &
                                4.5_rk,minimum=0.0_rk,vertical_movement=w_d, &
                                specific_light_extinction=kc)

   ! Register contribution of state to global aggregate variables.
   call self%add_to_aggregate_variable(standard_variables%total_nitrogen,self%id_d)

   ! Register dependencies on external state variables
   call self%register_state_dependency(self%id_mintarget,'mineralisation_target','mmol/m**3','sink for remineralized detritus')

   end subroutine initialize
!EOC

!-----------------------------------------------------------------------
!BOP
!
! !IROUTINE: Right hand sides of Detritus model
!
! !INTERFACE:
   subroutine do(self,_ARGUMENTS_DO_)
!
! !INPUT PARAMETERS:
   class (type_examples_npzd_det), intent(in)     :: self
   _DECLARE_ARGUMENTS_DO_
!
! !LOCAL VARIABLES:
   real(rk)                   :: d
!EOP
!-----------------------------------------------------------------------
!BOC
   ! Enter spatial loops (if any)
   _LOOP_BEGIN_

   ! Retrieve current (local) state variable values.
   _GET_(self%id_d,d) ! detritus

   ! Set temporal derivatives
   _SET_ODE_(self%id_d,-self%rdn*d)

   ! If an externally maintained NUT pool is present, add mineralisation to it
   _SET_ODE_(self%id_mintarget, self%rdn*d)

   ! Leave spatial loops (if any)
   _LOOP_END_

   end subroutine do
!EOC

!-----------------------------------------------------------------------
!BOP
!
! !IROUTINE: Right hand sides of Detritus model exporting production/destruction matrices
!
! !INTERFACE:
   subroutine do_ppdd(self,_ARGUMENTS_DO_PPDD_)
!
! !INPUT PARAMETERS:
   class (type_examples_npzd_det), intent(in)     :: self
   _DECLARE_ARGUMENTS_DO_PPDD_
!
! !LOCAL VARIABLES:
   real(rk)                   :: d
!EOP
!-----------------------------------------------------------------------
!BOC
   ! Enter spatial loops (if any)
   _LOOP_BEGIN_

   ! Retrieve current (local) state variable values.
   _GET_(self%id_d,d) ! detritus

   ! Assign destruction rates to different elements of the destruction matrix.
   ! By assigning with _SET_DD_SYM_ [as opposed to _SET_DD_], assignments to dd(i,j)
   ! are automatically assigned to pp(j,i) as well.
   _SET_DD_SYM_(self%id_d,self%id_mintarget,self%rdn*d)

   ! Leave spatial loops (if any)
   _LOOP_END_

   end subroutine do_ppdd
!EOC

!-----------------------------------------------------------------------

   end module examples_npzd_det

!-----------------------------------------------------------------------
! Copyright by the GOTM-team under the GNU Public License - www.gnu.org
!-----------------------------------------------------------------------
