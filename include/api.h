/*
 *  api.h
 *
 *  NIST PQC signature API.
 *  Created by Bassham, Lawrence E (Fed) on 9/6/17.
 *  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
 */

#ifndef api_h
#define api_h

#include "config_HFE.h"

#define CRYPTO_SECRETKEYBYTES SIZE_SK_BYTES
#define CRYPTO_PUBLICKEYBYTES SIZE_PK_BYTES
#define CRYPTO_BYTES SIZE_SIGN_HFE

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk);

int
crypto_sign(unsigned char *sm, unsigned long long *smlen,
            const unsigned char *m, unsigned long long mlen,
            const unsigned char *sk);

int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 const unsigned char *sm, unsigned long long smlen,
                 const unsigned char *pk);

#endif /* api_h */
