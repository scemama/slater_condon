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

integer function n_excitations(det1,det2,Nint)
  implicit none
  integer*8, intent(in)  :: det1(Nint,2), det2(Nint,2)
  integer  , intent(in)  :: Nint

  integer                :: l
 
  n_excitations = &
      popcnt(xor( det1(1,1), det2(1,1)) ) + &
      popcnt(xor( det1(1,2), det2(1,2)) )
 
  do l=2,Nint
    n_excitations = n_excitations + &
      popcnt(xor( det1(l,1), det2(l,1)) ) + &
      popcnt(xor( det1(l,2), det2(l,2)) )
  end do
  n_excitations = ishft(n_excitations,-1)

end
