/*
 * Benchmark of key generation, signing and verification: median time over
 * NB_TESTS runs. The random generator is the NIST DRBG, seeded with a fixed
 * value.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "rng.h"
#include "api.h"
#include <time.h>


static inline unsigned long long get_time_ns()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (unsigned long long)ts.tv_sec * 1000000000ULL
         + (unsigned long long)ts.tv_nsec;
}

static int cmp_ull(const void *a, const void *b)
{
    unsigned long long x = *(const unsigned long long *)a;
    unsigned long long y = *(const unsigned long long *)b;

    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}


int main(void)
{
    unsigned char entropy_input[48];

    unsigned char *pk = (unsigned char *)malloc(CRYPTO_PUBLICKEYBYTES);
    unsigned char *sk = (unsigned char *)malloc(CRYPTO_SECRETKEYBYTES);

    unsigned char msg[] = "Benchmark";
    unsigned long long mlen = sizeof(msg) - 1;

    unsigned char *sm = (unsigned char *)malloc(CRYPTO_BYTES + sizeof(msg));
    unsigned char m2[sizeof(msg)];

    if (pk == NULL || sk == NULL || sm == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(pk);
        free(sk);
        free(sm);
        return 1;
    }
    unsigned long long smlen, mlen2;
    unsigned long long start, end;


    /* RNG initialisation */
    for (int i = 0; i < 48; i++)
        entropy_input[i] = i;

    randombytes_init(entropy_input, NULL, 256);

#define NB_TESTS 100

unsigned long long keygen_times[NB_TESTS];
unsigned long long sign_times[NB_TESTS];
unsigned long long verify_times[NB_TESTS];

for (int t = 0; t < NB_TESTS; t++) {

    if (t % 10 == 0) {
        printf("%d\n", t);
    }

    /* Key Generation */
    start = get_time_ns();

    if (crypto_sign_keypair(pk, sk) != 0) {
        printf("Key generation failed\n");
        return 1;
    }

    end = get_time_ns();
    keygen_times[t] = end - start;


    /* Signature */
    start = get_time_ns();

    if (crypto_sign(sm, &smlen, msg, mlen, sk) != 0) {
        printf("Signature failed\n");
        return 1;
    }

    end = get_time_ns();
    sign_times[t] = end - start;


    /* Verification */
    start = get_time_ns();

    if (crypto_sign_open(m2, &mlen2, sm, smlen, pk) != 0) {
        printf("Verification failed\n");
        return 1;
    }

    end = get_time_ns();
    verify_times[t] = end - start;


    if (mlen != mlen2 || memcmp(msg, m2, mlen) != 0) {
        printf("Recovered message is incorrect\n");
        return 1;
    }
}
qsort(keygen_times, NB_TESTS, sizeof(unsigned long long), cmp_ull);
qsort(sign_times, NB_TESTS, sizeof(unsigned long long), cmp_ull);
qsort(verify_times, NB_TESTS, sizeof(unsigned long long), cmp_ull);

double median_keygen, median_sign, median_verify;

if (NB_TESTS % 2 == 0) {
    median_keygen = (keygen_times[NB_TESTS/2 - 1] + keygen_times[NB_TESTS/2]) / 2.0;
    median_sign   = (sign_times[NB_TESTS/2 - 1] + sign_times[NB_TESTS/2]) / 2.0;
    median_verify = (verify_times[NB_TESTS/2 - 1] + verify_times[NB_TESTS/2]) / 2.0;
} else {
    median_keygen = keygen_times[NB_TESTS/2];
    median_sign   = sign_times[NB_TESTS/2];
    median_verify = verify_times[NB_TESTS/2];
}

printf("\n========== %s benchmark ==========\n", HFEDRAGON ? "D-James" : "James");
printf("HFEn             : %d\n",HFEn);
printf("Tests            : %d\n", NB_TESTS);
printf("Median KeyGen    : %.3f ms\n", median_keygen / 1e6);
printf("Median Sign      : %.3f ms\n", median_sign / 1e6);
printf("Median Verify    : %.3f ms\n", median_verify / 1e6);
printf("Signature verification: SUCCESS\n");
free(pk);
free(sk);
free(sm);
return 0;
}