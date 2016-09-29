uint64_t n_excitations(**uint64_t det1, **uint64_t det2, uint64_t Nint){
    uint64_t n_excit = __builtin_popcountll(det1[0][0]^det2[0][0]) + __builtin_popcountll(det1[0][1]^det2[0][1])
    uint32_t l = 0;

    for (uint64_t l = 1; l < Nint; l++) {
        n_excit += __builtin_popcountll(det1[l][0]^det2[l][0]) + __builtin_popcountll(det1[l][1]^det2[l][1])
    }
    return nexcit>>1
}


subroutine get_single_excitation(det1,det2,exc,phase,Nint)
 integer, intent(in)  :: Nint
 integer*8, intent(in)  :: det1(Nint,2)
 integer*8, intent(in)  :: det2(Nint,2)
 integer, intent(out) :: exc(0:2,2,2)
 double precision, intent(out) :: phase
 integer :: tz, l, ispin, ishift, nperm, i, j, k, m, n, high, low
 integer*8 :: hole, particle, tmp
 double precision, parameter :: phase_dble(0:1) = (/ 1.d0, -1.d0 /)

 exc(0,1,1) = 0
 exc(0,2,1) = 0
 exc(0,1,2) = 0
 exc(0,2,2) = 0
 do ispin = 1,2
   ishift = -63
   do l=1,Nint
     ishift = ishift + 64
     if (det1(l,ispin) == det2(l,ispin)) cycle
     tmp = xor( det1(l,ispin), det2(l,ispin) )
     particle = iand(tmp, det2(l,ispin))
     hole     = iand(tmp, det1(l,ispin))
     if (particle /= 0_8) then
       tz = trailz(particle)
       exc(0,2,ispin) = 1
       exc(1,2,ispin) = tz+ishift
     end if
     if (hole /= 0_8) then
       tz = trailz(hole)
       exc(0,1,ispin) = 1
       exc(1,1,ispin) = tz+ishift
     end if

     if ( iand(exc(0,1,ispin),exc(0,2,ispin)) == 1 ) then
       low  = min(exc(1,1,ispin),exc(1,2,ispin))
       high = max(exc(1,1,ispin),exc(1,2,ispin))
       j = ishft(low-1,-6)+1
       n = iand(low,63)
       k = ishft(high-1,-6)+1
       m = iand(high,63)
       if (j==k) then
         nperm = popcnt(iand(det1(j,ispin), &
            iand( ibset(0_8,m-1)-1_8, ibclr(-1_8,n)+1_8 ) ))
       else
         nperm = popcnt(iand(det1(k,ispin), ibset(0_8,m-1)-1_8)) + &
                 popcnt(iand(det1(j,ispin), ibclr(-1_8,n) +1_8))
         do i=j+1,k-1
           nperm = nperm + popcnt(det1(i,ispin))
         end do
       end if
       phase = phase_dble(iand(nperm,1))
       return
     end if
   end do
 end do
end
