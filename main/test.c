#include <stdint.h>
#include <stdio.h>
#define NINT 2
#define NDET 10000
#define MO_NUM 105
#define LMAX 10000

void read_input(***det,*N_int,*coef,filename,filename2){
    FILE *file;
    file = fopen(filename, "r");
    uint8_t i = 0;
    uint8_t num_in;
    while(fscanf(file, "%d", &num_in) > 0) {
        if(i==0){
            N_int = &num_in
        } else{
            det[]
        }
        integers[i] = num;
        i++;
    }
    if (file) {
        fscanf(myFile, "%d",N_int);

        fclose(file);
    }

     open(unit=10,file=filename)
     read(10,*) Nint
     read(10,*) det
     close(10)
     open(unit=10,file=filename2)
     read(10,*) coef
     close(10)

}

double irp_rdtsc_(void) {
  unsigned long long a, d;
  __asm__ volatile ("rdtscp" : "=a" (a), "=d" (d));
  return (double)((d<<32) + a);
}


int main(){
    uint8_t det[NINT][2][NDET]
    uint8_t n_excitations;
    uint8_t i, j, k;
    uint8_t N_int;
    double phase;
    double t0, t1, irp_rdtsc;
    double cpu0, cpu1;
    double events;
    double error;
    double coef[NDET];
    double density_matrix; //density_matrix(mo_num,mo_num)
    uint8_t res;
    double nb[3];
    double t,t2,t3[3],t4[3];
    uint8_t exc[3][2][2];
    uint8_t det[NINT][2][NDET];
    read_input(&det,&N_int,&coef,'h2o_determinants.dat','cu.coef')

}
    call read_input(det,N_int,coef,ndet,'h2o_determinants.dat','cu.coef')

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
       res = n_excitations(det(1,1,l),det(1,1,k),N_int)
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
