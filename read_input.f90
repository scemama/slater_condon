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

subroutine read_input(det,Nint,coef,ndet,filename,filename2)

 implicit none
 character*(*), intent(in) :: filename, filename2
 integer  :: Nint, ndet
 double precision :: coef(ndet)
 integer*8 :: det(Nint,2,ndet)

 open(unit=10,file=filename)
 read(10,*) Nint
 read(10,*) det
 close(10)
 open(unit=10,file=filename2)
 read(10,*) coef
 close(10)
end
