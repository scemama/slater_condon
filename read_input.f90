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
