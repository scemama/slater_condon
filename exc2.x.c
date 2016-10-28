subroutine get_double_excitation(det1,det2,exc,phase,Nint)
 implicit none
 integer, intent(in)  :: Nint
 integer*8, intent(in)  :: det1(Nint,2), det2(Nint,2)
 integer, intent(out) :: exc(0:2,2,2)
 double precision, intent(out) :: phase
 integer :: l, ispin, idx_hole, idx_particle, ishift
 integer :: i,j,k,m,n,high, low,a,b,c,d,nperm,tz,nexc
 integer*8 :: hole, particle, tmp
 double precision, parameter :: phase_dble(0:1) = (/ 1.d0, -1.d0 /)
 exc(0,1,1) = 0
 exc(0,2,1) = 0
 exc(0,1,2) = 0
 exc(0,2,2) = 0
 nexc=0
 nperm=0
 do ispin = 1,2
  idx_particle = 0
  idx_hole = 0
  ishift = -63
  do l=1,Nint
   ishift = ishift + 64
   if (det1(l,ispin) == det2(l,ispin))  then
     cycle
   end if
   tmp = xor( det1(l,ispin), det2(l,ispin) )
   particle = iand(tmp, det2(l,ispin))
   hole     = iand(tmp, det1(l,ispin))
   do while (particle /= 0_8)
     tz = trailz(particle)
     nexc = nexc+1
     idx_particle = idx_particle + 1
     exc(0,2,ispin) = exc(0,2,ispin) + 1
     exc(idx_particle,2,ispin) = tz+ishift
     particle = iand(particle,particle-1_8)
   end do
   do while (hole /= 0_8)
     tz = trailz(hole)
     nexc = nexc+1
     idx_hole = idx_hole + 1
     exc(0,1,ispin) = exc(0,1,ispin) + 1
     exc(idx_hole,1,ispin) = tz+ishift
     hole = iand(hole,hole-1_8)
   end do
   if (nexc == 4) exit
  end do

  do i=1,exc(0,1,ispin)
    low  = min(exc(i,1,ispin),exc(i,2,ispin))
    high = max(exc(i,1,ispin),exc(i,2,ispin))
    j = ishft(low-1,-6)+1
    n = iand(low,63)
    k = ishft(high-1,-6)+1
    m = iand(high,63)
    if (j==k) then
      nperm = nperm + popcnt(iand(det1(j,ispin),  &
         iand( ibset(0_8,m-1)-1_8, ibclr(-1_8,n)+1_8 ) ))
    else
      nperm = nperm + popcnt(iand(det1(k,ispin),  &
                             ibset(0_8,m-1)-1_8)) &
                    + popcnt(iand(det1(j,ispin),  &
                             ibclr(-1_8,n) +1_8))
      do l=j+1,k-1
        nperm = nperm + popcnt(det1(l,ispin))
      end do
    end if
  end do
  if (exc(0,1,ispin) == 2) then
    a = min(exc(1,1,ispin), exc(1,2,ispin))
    b = max(exc(1,1,ispin), exc(1,2,ispin))
    c = min(exc(2,1,ispin), exc(2,2,ispin))
    d = max(exc(2,1,ispin), exc(2,2,ispin))
    if (c>a .and. c<b .and. d>b) nperm = nperm + 1
    exit
   end if
 end do
 phase = phase_dble(iand(nperm,1))

end
