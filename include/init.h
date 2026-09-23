#ifndef _INIT_H
#define _INIT_H

/* Error codes (negative) and macros to copy/clear/swap multi-word elements */

#include "add.h"
#include "arch.h"

#define ERROR_ALLOC (-2)
/* No valid root found for any salt during signing */
#define SIGNATURE_FAILED (-3)

/* Return ERROR_ALLOC from the calling function if an allocation failed */
#define VERIFY_ALLOC_RET(p) \
    if(!p) \
    {\
        return(ERROR_ALLOC);\
    }

/* Copy */
#define COPY64(c,a) \
    (c)[0]=(a)[0];

#define COPY128(c,a) \
    COPY64(c,a); \
    (c)[1]=(a)[1];

#define COPY192(c,a) \
    COPY128(c,a); \
    (c)[2]=(a)[2];

#define COPY256(c,a) \
    COPY192(c,a); \
    (c)[3]=(a)[3];

#define COPY320(c,a) \
    COPY256(c,a); \
    (c)[4]=(a)[4];

#define COPY384(c,a) \
    COPY320(c,a); \
    (c)[5]=(a)[5];

#define COPY448(c,a) \
    COPY384(c,a); \
    (c)[6]=(a)[6];

#define COPY512(c,a) \
    COPY448(c,a); \
    (c)[7]=(a)[7];

#define COPY576(c,a) \
    COPY512(c,a); \
    (c)[8]=(a)[8];

#define COPY640(c,a) \
    COPY576(c,a); \
    (c)[9]=(a)[9];

/* Set to 0 */
#define SET0_64(c) \
    (c)[0]=0UL;

#define SET0_128(c) \
    SET0_64(c);\
    (c)[1]=0UL;

#define SET0_192(c) \
    SET0_128(c);\
    (c)[2]=0UL;

#define SET0_256(c) \
    SET0_192(c);\
    (c)[3]=0UL;

#define SET0_320(c) \
    SET0_256(c);\
    (c)[4]=0UL;

#define SET0_384(c) \
    SET0_320(c);\
    (c)[5]=0UL;

#define SET0_448(c) \
    SET0_384(c);\
    (c)[6]=0UL;

#define SET0_512(c) \
    SET0_448(c);\
    (c)[7]=0UL;

#define SET0_576(c) \
    SET0_512(c);\
    (c)[8]=0UL;

#define SET0_640(c) \
    SET0_576(c);\
    (c)[9]=0UL;

/* Swap a and b (three xors) */
#define PERMUTATION(ADD2,a,b) \
    ADD2(a,b); \
    ADD2(b,a); \
    ADD2(a,b);

#define PERMUTATION64(a,b) PERMUTATION(ADD64_2,a,b);
#define PERMUTATION128(a,b) PERMUTATION(ADD128_2,a,b);
#define PERMUTATION192(a,b) PERMUTATION(ADD192_2,a,b);
#define PERMUTATION256(a,b) PERMUTATION(ADD256_2,a,b);
#define PERMUTATION320(a,b) PERMUTATION(ADD320_2,a,b);
#define PERMUTATION384(a,b) PERMUTATION(ADD384_2,a,b);
#define PERMUTATION448(a,b) PERMUTATION(ADD448_2,a,b);
#define PERMUTATION512(a,b) PERMUTATION(ADD512_2,a,b);
#define PERMUTATION576(a,b) PERMUTATION(ADD576_2,a,b);
#define PERMUTATION640(a,b) PERMUTATION(ADD640_2,a,b);

#endif
