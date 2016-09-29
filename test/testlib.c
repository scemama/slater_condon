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
    uint64_t temporary = __builtin_popcountll(11111000);
    printf("does print num work:  %" PRIu64 "\n", temporary);
    printf("is input working: %" PRIu64 "\n\n", Nint[0]);
    printf("array input: %" PRIu64 "\n", det1_alpha[0]);


    *n_excit = __builtin_popcountll(det1_alpha[0]^det2_alpha[0]) + __builtin_popcountll(det1_beta[0]^det2_beta[0]);

    printf("working\n");

    for (uint64_t l = 1; l < Nint[0]; l++) {
        *n_excit += __builtin_popcountll(det1_alpha[l]^det2_alpha[l]) + __builtin_popcountll(det1_beta[l]^det2_beta[l]);
    }
    printf("%" PRIuPTR "\n", *n_excit>>1);
}
