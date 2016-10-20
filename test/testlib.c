#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

///////////////////////////////////////////////////////////////////

void myprint(void);
uint64_t excindex(uint64_t *a_index, uint64_t *b_index, uint64_t *c_index);
uint64_t detindex(uint64_t *a_index, uint64_t *b_index, uint64_t *c_index, uint64_t *Ndets);
uint64_t densityindex(uint64_t *a_index, uint64_t *b_index, uint64_t *mo_num);
void get_excitation(uint64_t *det1, uint64_t *det2, uint64_t *exc, uintptr_t *degree, uint64_t *phase, uint64_t *Nint, uint64_t *dets, uint64_t *Ndets);
void get_single_excitation(uint64_t *det1, uint64_t *det2, uint64_t *exc, uint64_t *Nint, double *phase, uint64_t *dets, uint64_t *Ndets);
void get_double_excitation(uint64_t *det1, uint64_t *det2, uint64_t *exc, uint64_t *Nint, double *phase, uint64_t *dets, uint64_t *Ndets);
void n_excitations(uint64_t *det1, uint64_t *det2, uint64_t *Nint, uintptr_t n_excit, uint64_t *dets, uint64_t *Ndets);
void read_input(uint64_t *dets,uint64_t *Nint,double *coef,uint64_t *Ndets,char *filename,char *filename2);
void compute_density_matrix(uint64_t *dets, uint64_t *Ndets, double *coef, uint64_t *mo_num, uint64_t *Nint, double *density_matrix);
///////////////////////////////////////////////////////////////////

void myprint(){
    printf("hello world\n");
}

uint64_t excindex(uint64_t *a_index, uint64_t *b_index, uint64_t *c_index){
    return (*a_index*(uint64_t)2) + (*b_index*(uint64_t)2) + (c_index);
}

uint64_t detindex(uint64_t a_index, uint64_t b_index, uint64_t c_index, uint64_t *Ndets){
    return (*Ndets * *a_index) + (*b_index * (uint64_t)2) + *c_index;
}

uint64_t densityindex(uint64_t a_index, uint64_t b_index, uint64_t *mo_num){
    return (*mo_num * *a_index) + *b_index;
}

void get_excitation(uint64_t *det1, uint64_t *det2, uint64_t *exc, uintptr_t *degree, uint64_t *phase, uint64_t *Nint, uint64_t *dets, uint64_t *Ndets){
    n_excitations(det1, det2, Nint, degree, dets,Ndets);
    switch(*degree){
        case 2 :
            //call DOUBLE EXCITATION FUNCTION
            get_single_excitation(det1, det2, exc, Nint, phase, dets, Ndets);
            break;
        case 1 :
            //call SINGLE EXCITATION FUNCTION
            get_single_excitation(det1, det2, exc, Nint, phase, dets, Ndets);
            break;
        case 0 :
            break;
        default :
            degree = -1;
            break;
    }
}

void get_single_excitation(uint64_t *det1, uint64_t *det2, uint64_t *exc, uint64_t *Nint, double *phase, uint64_t *dets, uint64_t *Ndets){
    double phase_dble[2] = {1.0,-1.0};
    uint64_t hole, particle, tmp;
    int64_t temp_a = 0, temp_b = -1;
    uint64_t tz, nperm, i, j, k, m, n, high, low;
    int8_t ishift;
    exc[excindex(0, 0, 0)] = 0;
    exc[excindex(0, 1, 0)] = 0;
    exc[excindex(0, 0, 1)] = 0;
    exc[excindex(0, 1, 1)] = 0;
    for (uint64_t ispin = 0; ispin < 2; ispin++){
        ishift = -63;
        for (uint64_t l = 0; l < Nint; l++) {
            ishift += 64;
            if (dets[detindex(det1,&ispin,&l,Ndets)] == dets[detindex(det2,&ispin,&l,Ndets)]){
                continue;
            } else {
                tmp = dets[detindex(det1,&ispin,&l,Ndets)] ^ dets[detindex(det2,&ispin,&l,Ndets)];
                particle = tmp & dets[detindex(det2,&ispin,&l,Ndets)];
                hole = tmp & dets[detindex(det1,&ispin,&l,Ndets)];
                if(particle != 0){
                    tz = __builtin_ctz(particle);
                    exc[excindex(0,1,&ispin)] = 1;
                    exc[excindex(1,1,&ispin)] = tz+ishift;
                }
                if(hole != 0){
                    tz = __builtin_ctz(hole);
                    exc[excindex(0,1,&ispin)] = 1;
                    exc[excindex(1,1,&ispin)] = tz+ishift;
                }
                if((exc[excindex(0,0,&ispin)] & exc[excindex(0,1,&ispin)]) == 1){
                    low = MIN(exc[excindex(1,0,&ispin)],exc[excindex(1,1,&ispin)]);
                    high = MAX(exc[excindex(1,0,&ispin)],exc[excindex(1,1,&ispin)]);
                    j = ((low-1)<<6); //might need +1
                    n = low & 63;
                    k = ((high-1)<<6); //might need +1
                    m = high & 63;
                    if (j==k){
                        temp_a |= (1 << (m-1)) - 1;
                        temp_b &= ~(1 << n) + 1;
                        nperm = __builtin_popcountll( dets[detindex(det1,&ispin,&j,Ndets)] & (temp_a & temp_b));
                    } else {
                        temp_a |= (1 << (m-1)) - 1;
                        temp_b &= ~(1 << n) + 1;
                        nperm = __builtin_popcountll(dets[detindex(det1,&ispin,&k,Ndets)] & temp_a) + __builtin_popcountll(dets[detindex(det1,&ispin,&j,Ndets)] & temp_b);
                        for (uint64_t i = j ; i< k-1; i++){ //possible error
                          nperm += __builtin_popcountll(dets[detindex(det1,&ispin,&i,Ndets)]);
                        }
                    }
                    phase = &phase_dble[nperm & 1]; //could move further out?
                }
            }
        }
    }
}

void get_double_excitation(uint64_t *det1, uint64_t *det2, uint64_t *exc, uint64_t *Nint, double *phase, uint64_t *dets, uint64_t *Ndets){
    uint64_t idx_hole, idx_particle;
    int8_t ishift;
    uint64_t j,k,m,n,high,low,a,b,c,d,nperm = 0,tz,nexc = 0;
    int64_t temp_a = 0, temp_b = 0;
    uint64_t hole, particle, tmp;
    double phase_dble[2] = {1.0,-1.0};
    exc[excindex(0, 0, 0)] = 0;
    exc[excindex(0, 1, 0)] = 0;
    exc[excindex(0, 0, 1)] = 0;
    exc[excindex(0, 1, 1)] = 0;

    for (uint64_t ispin = 0; ispin < 2; ispin++){
        idx_particle = 0;
        idx_hole = 0;
        ishift = -63;
        for (uint64_t l = 0; l < Nint; l++) {
            ishift += 64;
            if (dets[detindex(det1,&ispin,&l,Ndets)] == dets[detindex(det2,&ispin,&l,Ndets)]){
                continue;
            } else {
                tmp = dets[detindex(det1,&ispin,&l,Ndets)] ^ dets[detindex(det2,&ispin,&l,Ndets)];
                particle = tmp & dets[detindex(det2,&ispin,&l,Ndets)];
                hole = tmp & dets[detindex(det1,&ispin,&l,Ndets)];
                while(particle != 0){
                    tz = __builtin_ctz(particle);
                    nexc += 1;
                    idx_particle += 1;
                    exc[excindex(0,1,&ispin)] += 1;
                    exc[excindex(&idx_particle,1,&ispin)] = tz+ishift;
                    particle = particle & (particle-1);
                }
                while(hole != 0){
                    tz = __builtin_ctz(hole);
                    nexc += 1;
                    idx_hole += 1;
                    exc[excindex(0,1,&ispin)] = 0;
                    exc[excindex(&idx_hole,1,&ispin)] = tz+ishift;
                    hole = hole & (hole-1);
                }
                if (nexc == 4){
                    break;
                }
            }
        }
        for( uint64_t i = 0; i < exc[excindex(0,1,&ispin)]; i++){
            low = MIN(exc[excindex(&i,0,&ispin)],exc[excindex(&i,1,&ispin)]);
            high = MAX(exc[excindex(&i,0,&ispin)],exc[excindex(&i,1,&ispin)]);
            j = ((low-1)<<6); //might need +1
            n = low & 63;
            k = ((high-1)<<6); //might need +1
            m = high & 63;
            if (j==k){
                temp_a |= (1 << (m-1)) - 1;
                temp_b &= ~(1 << n) + 1;
                nperm += __builtin_popcountll( dets[detindex(det1,&ispin,&j,Ndets)] & (temp_a & temp_b));
            } else {
                temp_a |= (1 << (m-1)) - 1;
                temp_b &= ~(1 << n) + 1;
                nperm += __builtin_popcountll(dets[detindex(det1,&ispin,&k,Ndets)] & temp_a) + __builtin_popcountll(dets[detindex(det1,&ispin,&j,Ndets)] & temp_b);
                for (uint64_t l = j ; l < k-1; l++){ //possible error
                  nperm += __builtin_popcountll(dets[detindex(det1,&ispin,&l,Ndets)]);
                }
            }
        }
        if(exc[excindex(0,1,&ispin)] == 2){
            a = MIN(exc[excindex(0,0,&ispin)],exc[excindex(0,1,&ispin)]);
            b = MAX(exc[excindex(0,0,&ispin)],exc[excindex(0,1,&ispin)]);
            c = MIN(exc[excindex(1,0,&ispin)],exc[excindex(1,1,&ispin)]);
            d = MAX(exc[excindex(1,0,&ispin)],exc[excindex(1,1,&ispin)]);
            if(c>a && c<b && d>b){
                nperm++;
            }
            break;
        }
    }
    *phase = phase_dble[nperm & 1];
}

void n_excitations(uint64_t *det1, uint64_t *det2, uint64_t *Nint, uintptr_t n_excit, uint64_t *dets, uint64_t *Ndets){
    n_excit = 0;
    for (uint64_t l = 0; l <= Nint; l++) {
        //printf("STEP: %" PRIu64 "\n", l);
        n_excit += __builtin_popcountll(dets[detindex(det1,0,&l, Ndets)] ^ dets[detindex(det2,0,&l,Ndets)]) + __builtin_popcountll(dets[detindex(det1,1,&l,Ndets)] ^ dets[detindex(det2,1,&l,Ndets)]);
    }
    n_excit = n_excit >> 1;
    //printf("Excitations : %" PRIuPTR "\n", *n_excit);
}

void read_input(uint64_t *dets,uint64_t *Nint,double *coef,uint64_t *Ndets,char *filename,char *filename2){
    FILE *f1 = fopen(filename, "r");
    char readin[500], stripEOL;
    fgets(readin, sizeof(readin), f1);
    //reading in is probably slower than it has to be beacuse the input is in fortran style and i am lazy
    stripEOL = getc(readin);
    Nint = (uint64_t)stripEOL;
    dets = calloc(*Ndets * (uint64_t)2 * *Nint ,sizeof(uint64_t));
    uint64_t count_det_num = 0;
    uint64_t count_spin_num = 0;
    uint64_t count_nint_num = 0;
    while (!feof(f1)){
        fgets(readin, sizeof(readin), f1);
        stripEOL = getc(readin);
        dets[detindex(&count_det_num,&count_spin_num,&count_nint_num,Ndets)] = (uint64_t)stripEOL;
        count_nint_num++;
        if (count_nint_num == Nint){
            count_nint_num = 0;
            count_spin_num++;
            if (count_spin_num == 2){
                count_spin_num = 0;
                count_det_num++;
            }
        }
    }
    FILE *f2 = fopen(filename2, "r");
    uint64_t count = 0;
    while (!feof(f2)){
        fgets(readin, sizeof(readin), f2);
        stripEOL = getc(readin);
        coef[count] = (double)stripEOL;
        count++;
    }
}
 double irp_rdtsc_(void) {
   unsigned long long a, d;
   __asm__ volatile ("rdtscp" : "=a" (a), "=d" (d));
   return (double)((d<<32) + a);
 }

void compute_density_matrix(uint64_t *dets, uint64_t *Ndets, double *coef, uint64_t *mo_num, uint64_t *Nint, double *density_matrix){
    uint64_t i,j,k,l,ispin,ishift;
    uint64_t buffer;
    uintptr_t *deg = 0;
    uint64_t *exc = (uint64_t *)malloc(8*sizeof(uint64_t));
    double *phase = 0, c;
    uint64_t *n_excits = 0;
    for(uint64_t k = 0; k < Ndets; k++){
        for(uint64_t ispin = 0; ispin < 2; ispin++){
            ishift = 0;
            for (uint64_t i = 0; i < Nint; i++){
                buffer = dets[detindex(&k,&ispin,&i,Ndets)];
                while(buffer != 0){
                    j = __builtin_ctz(buffer) + ishift; //possible error
                    density_matrix[densityindex(&j,&j,mo_num)] += coef[k] * coef[k];
                    buffer = buffer && (buffer-1);
                }
                ishift += 64;
            }
        }
        for(uint64_t l = 0; l < k-1; l++){
            n_excitations(&k, &l, Nint, n_excits, dets, Ndets);
            if (*n_excits != 1){
                continue;
            }
            get_excitation(&k, &l, exc, deg, phase,Nint, dets, Ndets);
            if(exc[excindex((uint64_t)0, (uint64_t)0, (uint64_t)0)] == 1){
                i = exc[excindex((uint64_t)1, (uint64_t)0, (uint64_t)0)];
                j = exc[excindex((uint64_t)1, (uint64_t)1, (uint64_t)0)];
            } else {
                i = exc[excindex((uint64_t)1, (uint64_t)0, (uint64_t)1)];
                j = exc[excindex((uint64_t)1, (uint64_t)1, (uint64_t)1)];
            }
            c = *phase*coef[k]*coef[l];
            density_matrix[densityindex(&j,&i,mo_num)] += c;
            density_matrix[densityindex(&i,&j,mo_num)] += c;
        }
    }
}
int main(void){
    uint64_t *Nint = 0, *dets = 0, Ndets = 10000;
    uint64_t mo_num = 105;
    double *coef = 0;
    char *filename =  "h2o_determinants.dat";
    char *filename2 =  "cu.coef";
    double *density_matrix;
    density_matrix = calloc((mo_num ) * (mo_num) , sizeof(double));
    myprint();
    read_input(dets, Nint, coef, &Ndets, filename, filename2);
    myprint();
    dets = calloc((*Nint) * 2 * (Ndets), sizeof(uint64_t));
}
