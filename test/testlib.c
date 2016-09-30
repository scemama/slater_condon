#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>


void myprint(void);
void n_excitations(uint64_t *det1_alpha, uint64_t *det1_beta, uint64_t *det2_alpha, uint64_t *det2_beta, uint64_t  *Nint, uintptr_t *n_excit);

void myprint()
{
    printf("hello world\n");
}



void n_excitations(uint64_t *det1_alpha, uint64_t *det1_beta, uint64_t *det2_alpha, uint64_t *det2_beta, uint64_t  *Nint, uintptr_t *n_excit)
{
    printf("alpha recieved: %" PRIu64 " , ", det1_alpha[0]);
    printf("%" PRIu64 "\n", det1_alpha[1]);
    printf("beta recieved: %" PRIu64 " , ", det1_beta[0]);
    printf("%" PRIu64 "\n", det1_beta[1]);

    printf("step 1: %u , ", __builtin_popcountll(det1_alpha[0]^det2_alpha[0]));
    printf("step 2: %u , ", __builtin_popcountll(det1_alpha[1]^det2_alpha[1]));
    printf("step 3: %u , ", __builtin_popcountll(det1_beta[0]^det2_beta[0]));
    printf("step 4: %u \n", __builtin_popcountll(det1_beta[1]^det2_beta[1]));


    *n_excit = 0;
    for (uint64_t l = 0; l <= *Nint; l++) {
        printf("STEP: %" PRIu64 "\n", l);
        *n_excit += __builtin_popcountll(det1_alpha[l]^det2_alpha[l]) + __builtin_popcountll(det1_beta[l]^det2_beta[l]);
    }
    printf("Excitations : %" PRIuPTR "\n", *n_excit>>1);
}
