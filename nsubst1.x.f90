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
