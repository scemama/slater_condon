subroutine get_excitation(det1,det2,exc,degree,phase,Nint)
 implicit none
 integer, intent(in)  :: Nint
 integer*8, intent(in)  :: det1(Nint,2), det2(Nint,2)
 integer, intent(out) :: exc(0:2,2,2)
 integer, intent(out) :: degree
 double precision, intent(out) :: phase

 integer :: n_excitations

 degree = n_excitations(det1,det2,Nint)

 select case (degree)
 
   case (3:)
     degree = -1
     return

   case (2)
     call get_double_excitation(det1,det2,exc,phase,Nint)
     return

   case (1)
     call get_single_excitation(det1,det2,exc,phase,Nint)
     return

   case(0)
     return

   end select
end

