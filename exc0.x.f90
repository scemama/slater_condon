!   Slater-Condon Rules
!   Copyright (C) 2013 Anthony Scemama <scemama@irsamc.ups-tlse.fr>
!                      Emmanuel Giner <emmanuel_giner_jr@hotmail.fr>
!
!   This program is free software; you can redistribute it and/or modify
!   it under the terms of the GNU General Public License as published by
!   the Free Software Foundation; either version 2 of the License, or
!   (at your option) any later version.
!
!   This program is distributed in the hope that it will be useful,
!   but WITHOUT ANY WARRANTY; without even the implied warranty of
!   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!   GNU General Public License for more details.
!
!   You should have received a copy of the GNU General Public License along
!   with this program; if not, write to the Free Software Foundation, Inc.,
!   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

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

