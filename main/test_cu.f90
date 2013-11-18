  program test
    implicit none
    integer,parameter  :: Nint=1, ndet=10000
    integer*8  :: det(Nint,2,ndet)
    integer :: n_excitations
    integer    :: i,k,l
    integer    :: N_int
    double precision :: phase
    integer    :: exc(0:2,2,2)
    double precision :: t0, t1, irp_rdtsc
    double precision :: t, t2, t3(-1:2), t4(-1:2)
    double precision :: nb(-1:2)
    double precision :: cpu0, cpu1
    double precision :: events
    double precision :: coef(ndet)
    double precision :: error
    integer, parameter :: mo_num = 49
    integer:: res
    double precision :: density_matrix(mo_num,mo_num)
    integer,parameter  :: lmax = 10000
    N_int = Nint
    call read_input(det,N_int,coef,ndet,'cu.dat','cu.coef')

    print *,  'ndet = ', ndet
    !-------------------------------------------------
    t=0.d0
    t2=0.d0
    call cpu_time(cpu0)
    events = 0_8
    do l=1,ndet
     i=0
     t0 = irp_rdtsc()
     do k=1,ndet
       !DIR$ FORCEINLINE
       res=n_excitations(det(1,1,l),det(1,1,k),N_int)
     enddo
     t1 = irp_rdtsc(i)
     t = t+(t1-t0)
     t2 = t2+(t1-t0)*(t1-t0)
    enddo
    events = dble(ndet)**2
    call cpu_time(cpu1)
    error = sqrt( abs((t/events)**2-t2/events)/events )
    print *,  'Cycles n_excitations:',  t/events, ' +/- ', error/sqrt(events)
    print *,  'CPU    n_excitations:',  (cpu1-cpu0)
    print *,  res
    !-------------------------------------------------


    !-------------------------------------------------
    t=0.d0
    t2=0.d0
    call cpu_time(cpu0)
    do l=1,ndet
     t0 = irp_rdtsc()
     do k=1,ndet
      !DIR$ FORCEINLINE
      call get_excitation(det(1,1,l),det(1,1,k),exc,i,phase,N_int)
     enddo
     t1 = irp_rdtsc(i)
     t = t+(t1-t0)
     t2 = t2+(t1-t0)*(t1-t0)
    enddo
    events = dble(ndet)**2
    call cpu_time(cpu1)
    error = sqrt( abs((t/events)**2-t2/events)/events )
    print *,  'Cycles get_excitation:',  t/events, ' +/- ', error/sqrt(events)
    print *,  'CPU    get_excitation:',  (cpu1-cpu0)
    !-------------------------------------------------
!   stop

    !-------------------------------------------------
    t=0.d0
    t3=0.d0
    t4=0.d0
    do l=1,ndet-1
     t0 = irp_rdtsc()
     do k=1,lmax
      !DIR$ FORCEINLINE
      call get_excitation(det(1,1,l),det(1,1,1),exc,i,phase,N_int)
     enddo
     if (i==1) then
     do k=lmax+1,lmax*100
      !DIR$ FORCEINLINE
      call get_excitation(det(1,1,l),det(1,1,1),exc,i,phase,N_int)
     enddo
     endif
     t1 = irp_rdtsc()
     t3(i) = t3(i)+(t1-t0)
     t4(i) = t4(i)+(t1-t0)*(t1-t0)
     nb(i) = nb(i) + (k-1.d0)

     t0 = irp_rdtsc()
     do k=1,lmax
      !DIR$ FORCEINLINE
      call get_excitation(det(1,1,l),det(1,1,l+1),exc,i,phase,N_int)
     enddo
     if (i==1) then
     do k=lmax+1,lmax*100
      !DIR$ FORCEINLINE
      call get_excitation(det(1,1,l),det(1,1,l+1),exc,i,phase,N_int)
     enddo
     endif
     t1 = irp_rdtsc()
     t3(i) = t3(i)+(t1-t0)
     t4(i) = t4(i)+(t1-t0)*(t1-t0)
     nb(i) = nb(i) + (k-1.d0)


     t0 = irp_rdtsc()
     do k=1,lmax
      !DIR$ FORCEINLINE
      call get_excitation(det(1,1,l),det(1,1,ndet-l+1),exc,i,phase,N_int)
     enddo
     if (i==1) then
     do k=lmax+1,lmax*100
      !DIR$ FORCEINLINE
      call get_excitation(det(1,1,l),det(1,1,ndet-l+1),exc,i,phase,N_int)
     enddo
     endif
     t1 = irp_rdtsc()
     t3(i) = t3(i)+(t1-t0)
     t4(i) = t4(i)+(t1-t0)*(t1-t0)
     nb(i) = nb(i) + (k-1.d0)


     t0 = irp_rdtsc()
     do k=1,lmax/10
      !DIR$ FORCEINLINE
      call get_excitation(det(1,1,l),det(1,1,l),exc,i,phase,N_int)
     enddo
     t1 = irp_rdtsc()
     t3(0) = t3(0)+(t1-t0)
     t4(0) = t4(0)+(t1-t0)*(t1-t0)
     nb(0) = nb(0) + (k-1.d0)
    enddo
    error = sqrt( abs((t3(0)/nb(0))**2-t4(0)/nb(0))/nb(0) )
    print *,  'Cycles zero   :',  t3(0)/nb(0), ' +/- ', error/sqrt(nb(0))!, nb(0)
    error = sqrt( abs((t3(1)/nb(1))**2-t4(1)/nb(1))/nb(1) )
    print *,  'Cycles single :',  t3(1)/nb(1), ' +/- ', error/sqrt(nb(1))!, nb(1)
    error = sqrt( abs((t3(2)/nb(2))**2-t4(2)/nb(2))/nb(2) )
    print *,  'Cycles double :',  t3(2)/nb(2), ' +/- ', error/sqrt(nb(2))!, nb(2)
    error = sqrt( abs((t3(-1)/nb(-1))**2-t4(-1)/nb(-1))/nb(-1) )
    print *,  'Cycles other  :',  t3(-1)/nb(-1), ' +/- ', error/sqrt(nb(-1))!, nb(-1)
    !-------------------------------------------------

    call cpu_time(cpu0)
    t0 = irp_rdtsc()
    call compute_density_matrix(det,ndet,coef,mo_num, &
                   N_int,density_matrix)
    t1 = irp_rdtsc()
    call cpu_time(cpu1)
    print *,  'Cycles density matrix : ', (t1-t0)/(ndet*(ndet-1)/2)
    print *,  'CPU    density matrix : ', cpu1-cpu0
!   print *, (density_matrix(k,k), k=1,mo_num)


  end !----------

subroutine print_key( key, N_int  )
!!!! print the physical meaning of "key(N_int,2)" in an explicit way
 implicit none
 integer :: N_int
 integer*8, intent(in)    :: key(N_int,2)
 integer, parameter       :: alpha = 1
 integer, parameter       :: beta  = 2

 integer :: i, j, k, ibuf
 integer*8 :: itemp
 character*(1) :: buffer(10000)

 ibuf = 1
 do k=alpha,beta
   do i=1,N_int
    itemp = 1
    do j=1,64
      if (iand(itemp,key(i,k)) == itemp) then
        buffer(ibuf) = '1'
      else
        buffer(ibuf) = '.'
      endif
      ibuf = ibuf+1
      itemp = ishft(itemp,1)
    enddo
   enddo
 enddo
 print *, key(:,1), key(:,2)
 print *, buffer(1:ibuf)
 print *,  ''

end

