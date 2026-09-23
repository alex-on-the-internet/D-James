#ifndef _CONFIG_HFE_H
#define _CONFIG_HFE_H

#include "arch.h"
#include "bit.h"
#include "types_HFE.h"
#include <stdlib.h>

/****************** C++ compatibility ******************/

#ifdef __cplusplus
    #define BEGIN_EXTERNC extern "C" {
    #define END_EXTERNC }
#else
    #define BEGIN_EXTERNC
    #define END_EXTERNC
#endif

/****************** PARAMETERS ******************/

/* Notation of the paper: n=HFEn, a=HFEDELTA, m=HFEm=n-a, r=HFEIP,
   D=HFEDeg, ny=HASH_LENGTH. The field is GF(2) (q=2). */

/* Parameter set, chosen at compile time with one of the macros D_JAMES_128,
   D_JAMES_256, JAMES_128 or JAMES_256 (see the Makefile). Each one defines:
     K            classical security level in bits. It selects the hash
                  function; for D-James the number of hash variables is ny=2K.
     HFEn         degree n of the extension field GF(2^n). Supported range:
                  65 <= n <= 640, and config_gf2n.h must contain an irreducible
                  polynomial for n.
     HFEDELTA     number of minus equations a: the public key has m=n-a
                  equations.
     HFEDRAGON    1 for D-James (Dragon terms), 0 for James. */
#if defined(D_JAMES_128)
    #define K 128U
    #define HFEn 189U
    #define HFEDELTA 27U
    #define HFEDRAGON 1U
#elif defined(D_JAMES_256)
    #define K 256U
    #define HFEn 390U
    #define HFEDELTA 66U
    #define HFEDRAGON 1U
#elif defined(JAMES_128)
    #define K 128U
    #define HFEn 283U
    #define HFEDELTA 27U
    #define HFEDRAGON 0U
#elif defined(JAMES_256)
    #define K 256U
    #define HFEn 578U
    #define HFEDELTA 66U
    #define HFEDRAGON 0U
#else
    #error "Select a parameter set: D_JAMES_128, D_JAMES_256, JAMES_128 or JAMES_256"
#endif

/* Number of internal perturbation (IP) modifiers r. */
#define HFEIP 2U

/* Degree of the central polynomial: D = 2^HFEDegI + 2^HFEDegJ, HFEDegI>HFEDegJ. */
#define HFEDeg 5U
#define HFEDegI 2U
#define HFEDegJ 0U

/* D-James only. 1: the one-byte salt is appended to the signature. 0 (default):
   the salt is omitted, so the signature has n bits, and the verifier tries the
   256 possible values. */
#ifndef DRAGON_EXPLICIT_SALT
    #define DRAGON_EXPLICIT_SALT 0U
#endif

#define RIGHT_MULTIPLICATION_BY_T 1U

/* Number of public equations */
#define HFEm (HFEn-HFEDELTA)

/****************** VERIFICATION OF THE PARAMETERS ******************/

#if ((HFEn<65)||(HFEn>640))
    #error "HFEn must be in [65,640]"
#endif

#if (HFEIP<1)
    #error "HFEIP must be at least 1"
#endif

#if (HFEIP>=HFEn)
    #error "HFEIP must be strictly smaller than HFEn"
#endif

#if (!HFEm)
    #error "HFEm must be different of zero"
#endif

#if (HFEm>HFEn)
    #error "HFEDELTA must not exceed HFEn"
#endif

#if (HFEm>(K<<1))
    #error "HFEm>2K is not possible for this implementation"
#endif

#if (HFEDeg!=((1U<<HFEDegI)+(1U<<HFEDegJ)))
    #error "HFEDeg!=2^(HFEDegI) + 2^(HFEDegJ)"
#endif

#if (HFEDegI<=HFEDegJ)
    #error "Requirement: HFEDegI>HFEDegJ"
#endif

/****************** DRAGON TERMS ******************/

/* The Dragon block must come before any #if that uses HFEN or HASH_LENGTH:
   #if directives are evaluated immediately, in file order. */
#if HFEDRAGON
    /* ny: number of hash variables Y, equal to the hash output size in bits */
    #define HASH_LENGTH (K<<1)
    #define MAX_SALT 256U
#else
    #define HASH_LENGTH 0U
    #define MAX_SALT 0U
#endif

/* Number of variables of the public quadratic form: the n variables x, plus
   the ny hash variables Y for D-James (HFEN==HFEn for James). */
#define HFEN (HFEn+HASH_LENGTH)

#if (HFEDRAGON && DRAGON_EXPLICIT_SALT)
    #define SIZE_SALT_BYTES 1U
#else
    #define SIZE_SALT_BYTES 0U
#endif

/* Irreducible polynomial K1, K2, K3 defining GF(2^HFEn) */
#include "config_gf2n.h"

/****************** HASH FUNCTION ******************/

#if (K<=128)
    #define HASH SHA3_256
    #define SIZE_DIGEST_UINT 4
#elif (K<=192)
    #define HASH SHA3_384
    #define SIZE_DIGEST_UINT 6
#else
    #define HASH SHA3_512
    #define SIZE_DIGEST_UINT 8
#endif

/****************** SIZES ******************/

/* Public key (words and bytes) */
#define SIZE_PK MQ_GFqm_SIZE
#define SIZE_PK_BYTES (SIZE_PK<<3)

/* Secret key: HFE polynomial, HFE-IP data (MZ,MBilin,HQ), Dragon data
   (DragonL), S^(-1) and T^(-1) */
#define SIZE_SK (NB_UINT_HFEPOLY+NB_UINT_IP+NB_UINT_DRAGON+MATRIXn_SIZE+MATRIXn_SIZE)
#define SIZE_SK_BYTES (SIZE_SK<<3)

/* Signature: HFEn bits, plus the salt byte if DRAGON_EXPLICIT_SALT */
#define SIZE_SIGN_UNCOMPRESSED NB_WORD_GFqn
/* (x+7)/8 = Ceiling(x/8) */
#define SIZE_SIGN_HFE (((HFEn+7)>>3)+SIZE_SALT_BYTES)

/****************** MACROS FOR n ******************/

/* n = NB_BITS_UINT*quo + rem */
#define HFEnq (HFEn/NB_BITS_UINT)
#define HFEnr (HFEn%NB_BITS_UINT)
/* Number of words of an element of GF(2^n) */
#if (HFEnr)
    #define NB_WORD_GFqn (HFEnq+1)
#else
    #define NB_WORD_GFqn HFEnq
#endif

/* Mask for arithmetic in GF(2^n) */
#define HFE_MASKn mask64(HFEnr)

#define HFEnr8 (HFEn&7)
#define HFE_MASKn8 ((1U<<HFEnr8)-1)
/* Number of bytes of an element of GF(2^n) */
#define NB_BYTES_GFqn ((HFEn>>3)+((HFEnr8)?1:0))

/* Number of monomials, constant included, of a quadratic form in n variables
   (secret form without the Dragon terms) */
#define NB_MONOM (((HFEn*(HFEn+1))>>1)+1)

/* Number of monomials of the public quadratic form in the variables x (n) and
   Y (ny): the constant, the products x_i x_j (i<=j) and the products x_i Y_j.
   The Dragon terms are bilinear, so there is no Y_i Y_j nor Y_j term. */
#define NB_MONOM_PK (((HFEn*(2*HFEN-HFEn+1))>>1)+1)

/* Size of a quadratic form with coefficients in GF(2^n) */
#define MQn_GFqn_SIZE (NB_MONOM_PK*NB_WORD_GFqn)
#define MALLOC_MQSn (UINT*)malloc(MQn_GFqn_SIZE*sizeof(UINT))

/* Number of words of a matrix n*n in GF(2) */
#define MATRIXn_SIZE (HFEn*NB_WORD_GFqn)
#define MALLOC_MATRIXn (UINT*)malloc(MATRIXn_SIZE*sizeof(UINT))

/* Size (words) of a lower triangular matrix n*n */
#define LTRIANGULAR_N_SIZE (((HFEnq*(HFEnq+1))>>1)*NB_BITS_UINT+\
                            NB_WORD_GFqn*HFEnr)

/* Number of words of an element of GF(2^N), N=HFEN, and mask of its last word */
#define HFENq (HFEN/NB_BITS_UINT)
#define HFENr (HFEN%NB_BITS_UINT)
#if (HFENr)
    #define NB_WORD_GFqN (HFENq+1)
#else
    #define NB_WORD_GFqN HFENq
#endif

/****************** MACROS FOR m ******************/

/* m = NB_BITS_UINT*quo + rem */
#define HFEmq (HFEm/NB_BITS_UINT)
#define HFEmr (HFEm%NB_BITS_UINT)
/* Number of words of an element of GF(2^m) */
#if (HFEmr)
    #define NB_WORD_GFqm (HFEmq+1)
#else
    #define NB_WORD_GFqm HFEmq
#endif

/* Mask to truncate the last word */
#define HFE_MASKm mask64(HFEmr)

#define HFEmq8 (HFEm>>3)
#define HFEmr8 (HFEm&7U)
/* Number of bytes of an element of GF(2^m) */
#define NB_BYTES_GFqm (HFEmq8+((HFEmr8)?1:0))

/* The public key is stored equation by equation, each one as a vector of
   NB_MONOM_PK bits (see sign_keypairHFE.cpp): m*NB_MONOM_PK bits in total,
   plus at most 63 bits of padding per equation. */
#define NB_WORD_MONOM_PK ((NB_MONOM_PK+NB_BITS_UINT-1)/NB_BITS_UINT)
#define MQ_GFqm_SIZE (HFEm*NB_WORD_MONOM_PK)

/* During key generation, the public key is first computed monomial by
   monomial: NB_MONOM_PK vectors of m bits */
#define MQ_MONOM_SIZE (NB_MONOM_PK*NB_WORD_GFqm)


/****************** HFE POLYNOMIAL ******************/

/* Stored coefficients: X^0, X^1, X^(2^i+2^j) for i<HFEDegI (j<=i) and for
   i=HFEDegI (j<HFEDegJ). The leading term X^HFEDeg is monic and not stored. */
#define NB_COEFS_HFEPOLY (2+HFEDegJ+((HFEDegI*(HFEDegI+1))>>1))
#define NB_UINT_HFEPOLY (NB_COEFS_HFEPOLY*NB_WORD_GFqn)
#define MALLOC_HFEPOLY (UINT*)malloc(NB_UINT_HFEPOLY*sizeof(UINT))

/****************** SIZES FOR HFE-IP ******************/

/* Secret data of the IP modifier, stored after the HFE polynomial:
     MZ     : HFEIP*HFEn bits         (z = MZ.x)
     MBilin : (HFEDegI+1)*HFEIP elements of GF(2^n)  (coefficients of X^(2^t)*z_j)
     HQ     : NB_MONOM_IP elements of GF(2^n)        (triangular quadratic form in z) */

/* Number of pairs i<=j among the HFEIP hidden variables z */
#define NB_MONOM_IP ((HFEIP*(HFEIP+1))>>1)
#define MATRIXip_SIZE (HFEIP*NB_WORD_GFqn)
#define MLip_GFqn_SIZE ((HFEDegI+1)*HFEIP*NB_WORD_GFqn)
#define MQip_GFqn_SIZE (NB_MONOM_IP*NB_WORD_GFqn)
#define NB_UINT_IP (MATRIXip_SIZE+MLip_GFqn_SIZE+MQip_GFqn_SIZE)

/****************** SIZES FOR THE DRAGON TERMS ******************/

/* DragonL, stored after the IP data: (HFEDegI+1)*HASH_LENGTH elements of
   GF(2^n), coefficients of X^(2^t)*Y_j. Empty for James. */
#define NB_UINT_DRAGON ((HFEDegI+1)*HASH_LENGTH*NB_WORD_GFqn)

/****************** ARITHMETIC IN GF(2^n) ******************/

#define NB_WORD_MUL (NB_WORD_GFqn<<1)
#define HFEMULMOD void hfemulmod(uint64_t res[NB_WORD_GFqn], \
                const uint64_t A[NB_WORD_GFqn], const uint64_t B[NB_WORD_GFqn])
#define HFESQUAREMOD void hfesquaremod(uint64_t res[NB_WORD_GFqn], \
                                       const uint64_t A[NB_WORD_GFqn])

#if (NB_WORD_GFqn >= 2)
    #define KI (HFEn&63U)
#endif

#if (HFEn>=97)
    #define KI64 (64U-KI)
#endif

#if (NB_WORD_GFqn >= 3)
    #define K364 (64U-K3)

    #ifdef __PENTANOMHFE__
        #define K164 (64U-K1)
        #define K264 (64U-K2)
    #endif
#endif

#if (NB_WORD_GFqn == 2)
    #define HFEADD2 ADD128_2

    #define HFECOPY COPY128
    #define HFEPERMUTATION PERMUTATION128
    #define HFESET0 SET0_128

    #define HFEISZERO ISZERO128
    #define HFECMP_LT CMP_LT128
    #define HFECMP_GT CMP_GT128
    #define HFEDOTPRODUCT DOTPRODUCT128
    #define HFEDOTPRODUCTN DOTPRODUCT128M

    #if (HFEn<97)
        #define hfemul mul96
        #define hfesquare square96

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM96_TRINOM(P,Pol,K3,KI,Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #define HFEREM(P,Pol,Q,R) REM96_PENTANOM(P,Pol,K1,K2,K3,KI,K164,\
                                                     K264,K364,Q,R,HFE_MASKn)
        #endif
    #else
        #define hfemul mul128
        #define hfesquare square128

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM128_TRINOM(P,Pol,K3,KI,KI64,Q,R,\
                                                    HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #if (HFEnr)
                #define HFEREM(P,Pol,Q,R) REM128_PENTANOM(P,Pol,K1,K2,K3,KI,\
                                                          KI64,Q,R,HFE_MASKn)
            #else
                /* HFEn == 128 */
                #define HFEREM(P,Pol,Q,R) REM128_PENTANOM_K128(P,Pol,K1,K2,K3,R)
            #endif
        #endif
    #endif

#elif (NB_WORD_GFqn == 3)
    #define HFEADD2 ADD192_2

    #define HFECOPY COPY192
    #define HFEPERMUTATION PERMUTATION192
    #define HFESET0 SET0_192

    #define HFEISZERO ISZERO192
    #define HFECMP_LT CMP_LT192
    #define HFECMP_GT CMP_GT192
    #define HFEDOTPRODUCT DOTPRODUCT192
    #define HFEDOTPRODUCTN DOTPRODUCT192M

    #if (HFEn<161)
        #define hfemul mul160
        #define hfesquare square160

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM160_TRINOM(P,Pol,K3,KI,KI64,K364,Q,R,\
                                                    HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #define HFEREM(P,Pol,Q,R) REM160_PENTANOM(P,Pol,K1,K2,K3,KI,KI64,\
                                                  K164,K264,K364,Q,R,HFE_MASKn)
        #endif
    #else
        #define hfemul mul192
        #define hfesquare square192

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM192_TRINOM(P,Pol,K3,KI,KI64,K364,Q,R,\
                                                    HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #if (HFEnr)
                #define HFEREM(P,Pol,Q,R) REM192_PENTANOM(P,Pol,K1,K2,K3,KI,\
                                             KI64,K164,K264,K364,Q,R,HFE_MASKn)
            #else
                /* HFEn == 192 */
                #define HFEREM(P,Pol,Q,R) REM192_PENTANOM_K192(P,Pol,K1,K2,K3,\
                                                               K164,K264,K364,R)
            #endif
        #endif
    #endif

#elif (NB_WORD_GFqn == 4)
    #define HFEADD2 ADD256_2

    #define HFECOPY COPY256
    #define HFEPERMUTATION PERMUTATION256
    #define HFESET0 SET0_256

    #define HFEISZERO ISZERO256
    #define HFECMP_LT CMP_LT256
    #define HFECMP_GT CMP_GT256
    #define HFEDOTPRODUCT DOTPRODUCT256
    #define HFEDOTPRODUCTN DOTPRODUCT256M

    #if (HFEn<225)
        #define hfemul mul224
        #define hfesquare square224

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM224_TRINOM(P,Pol,K3,KI,KI64,K364,Q,R,\
                                                    HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #define HFEREM(P,Pol,Q,R) REM224_PENTANOM(P,Pol,K1,K2,K3,KI,KI64,\
                                                  K164,K264,K364,Q,R,HFE_MASKn)
        #endif
    #else
        #define hfemul mul256
        #define hfesquare square256

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM256_TRINOM(P,Pol,K3,KI,KI64,K364,Q,R,\
                                                    HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #if (HFEnr)
                #define HFEREM(P,Pol,Q,R) REM256_PENTANOM(P,Pol,K1,K2,K3,KI,\
                                             KI64,K164,K264,K364,Q,R,HFE_MASKn)
            #else
                /* HFEn == 256 */
                #define HFEREM(P,Pol,Q,R) REM256_PENTANOM_K256(P,Pol,K1,K2,K3,\
                                                               K164,K264,K364,R)
            #endif
        #endif
    #endif
#elif (NB_WORD_GFqn == 5)
    #define HFEADD2 ADD320_2

    #define HFECOPY COPY320
    #define HFEPERMUTATION PERMUTATION320
    #define HFESET0 SET0_320

    #define HFEISZERO ISZERO320
    #define HFECMP_LT CMP_LT320
    #define HFECMP_GT CMP_GT320
    #define HFEDOTPRODUCT DOTPRODUCT320
    #define HFEDOTPRODUCTN DOTPRODUCT320M

    #if (HFEn<289)
        #define hfemul mul288
        #define hfesquare square288

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM288_TRINOM(P,Pol,K3,KI,KI64,K364,\
                                                    Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #define HFEREM(P,Pol,Q,R) REM288_PENTANOM(P,Pol,K1,K2,K3,KI,KI64,\
                                                  K164,K264,K364,Q,R,HFE_MASKn)
        #endif
    #else
        #define hfemul mul320
        #define hfesquare square320

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM320_TRINOM(P,Pol,K3,KI,KI64,K364,Q,R,\
                                                    HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #if (HFEnr)
                #define HFEREM(P,Pol,Q,R) REM320_PENTANOM(P,Pol,K1,K2,K3,KI,\
                                             KI64,K164,K264,K364,Q,R,HFE_MASKn)
            #else
                /* HFEn == 320 */
                #define HFEREM(P,Pol,Q,R) REM320_PENTANOM_K320(P,Pol,K1,K2,K3,\
                                                               K164,K264,K364,R)
            #endif
        #endif
    #endif
#elif (NB_WORD_GFqn == 6)
    #define HFEADD2 ADD384_2

    #define HFECOPY COPY384
    #define HFEPERMUTATION PERMUTATION384
    #define HFESET0 SET0_384

    #define HFEISZERO ISZERO384
    #define HFECMP_LT CMP_LT384
    #define HFECMP_GT CMP_GT384
    #define HFEDOTPRODUCT DOTPRODUCT384
    #define HFEDOTPRODUCTN DOTPRODUCT384M

    #if (HFEn<353)
        #define hfemul mul352
        #define hfesquare square352

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM352_TRINOM(P,Pol,K3,KI,KI64,K364,Q,R,\
                                                    HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #define HFEREM(P,Pol,Q,R) REM352_PENTANOM(P,Pol,K1,K2,K3,KI,KI64,\
                                                  K164,K264,K364,Q,R,HFE_MASKn)
        #endif
    #else
        #define hfemul mul384
        #define hfesquare square384

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM384_TRINOM(P,Pol,K3,KI,KI64,K364,\
                                                    Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #if (HFEnr)
                #define HFEREM(P,Pol,Q,R) REM384_PENTANOM(P,Pol,K1,K2,K3,KI,\
                                             KI64,K164,K264,K364,Q,R,HFE_MASKn)
            #else
                /* HFEn == 384 */
                #define HFEREM(P,Pol,Q,R) REM384_PENTANOM_K384(P,Pol,K1,K2,K3,\
                                                               K164,K264,K364,R)
            #endif
        #endif
    #endif

#elif (NB_WORD_GFqn == 7)
    #define HFEADD2 ADD448_2

    #define HFECOPY COPY448
    #define HFEPERMUTATION PERMUTATION448
    #define HFESET0 SET0_448

    #define HFEISZERO ISZERO448
    #define HFECMP_LT CMP_LT448
    #define HFECMP_GT CMP_GT448
    #define HFEDOTPRODUCT DOTPRODUCT448
    #define HFEDOTPRODUCTN DOTPRODUCT448M

    #if (HFEn < 417)
        #define hfemul mul416
        #define hfesquare square416

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM416_TRINOM(P,Pol,K3,KI,KI64,K364,\
                                                    Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #define HFEREM(P,Pol,Q,R) REM416_PENTANOM(P,Pol,K1,K2,K3,KI,KI64,\
                                                  K164,K264,K364,Q,R,HFE_MASKn)
        #endif
    #else
        #define hfemul mul448
        #define hfesquare square448

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM448_TRINOM(P,Pol,K3,KI,KI64,K364,\
                                                    Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #if (HFEnr)
                #define HFEREM(P,Pol,Q,R) REM448_PENTANOM(P,Pol,K1,K2,K3,KI,\
                                             KI64,K164,K264,K364,Q,R,HFE_MASKn)
            #else
                /* HFEn == 448 */
                #define HFEREM(P,Pol,Q,R) REM448_PENTANOM_K448(P,Pol,K1,K2,K3,\
                                                               K164,K264,K364,R)
            #endif
        #endif
    #endif

#elif (NB_WORD_GFqn == 8)
    #define HFEADD2 ADD512_2

    #define HFECOPY COPY512
    #define HFEPERMUTATION PERMUTATION512
    #define HFESET0 SET0_512

    #define HFEISZERO ISZERO512
    #define HFECMP_LT CMP_LT512
    #define HFECMP_GT CMP_GT512
    #define HFEDOTPRODUCT DOTPRODUCT512
    #define HFEDOTPRODUCTN DOTPRODUCT512M

    #if (HFEn < 481)
        #define hfemul mul480
        #define hfesquare square480

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM480_TRINOM(P,Pol,K3,KI,KI64,K364,\
                                                    Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #define HFEREM(P,Pol,Q,R) REM480_PENTANOM(P,Pol,K1,K2,K3,KI,KI64,\
                                                  K164,K264,K364,Q,R,HFE_MASKn)
        #endif
    #else
        #define hfemul mul512
        #define hfesquare square512

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM512_TRINOM(P,Pol,K3,KI,KI64,K364,\
                                                    Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #if (HFEnr)
                #define HFEREM(P,Pol,Q,R) REM512_PENTANOM(P,Pol,K1,K2,K3,KI,\
                                             KI64,K164,K264,K364,Q,R,HFE_MASKn)
            #else
                /* HFEn == 512 */
                #define HFEREM(P,Pol,Q,R) REM512_PENTANOM_K512(P,Pol,K1,K2,K3,\
                                                               K164,K264,K364,R)
            #endif
        #endif
    #endif

#elif (NB_WORD_GFqn == 9)
    #define HFEADD2 ADD576_2

    #define HFECOPY COPY576
    #define HFEPERMUTATION PERMUTATION576
    #define HFESET0 SET0_576

    #define HFEISZERO ISZERO576
    #define HFECMP_LT CMP_LT576
    #define HFECMP_GT CMP_GT576
    #define HFEDOTPRODUCT DOTPRODUCT576
    #define HFEDOTPRODUCTN DOTPRODUCT576M

    #if (HFEn < 545)
        #define hfemul mul544
        #define hfesquare square544

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM544_TRINOM(P,Pol,K3,KI,KI64,K364,\
                                                    Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
                    #define HFEREM(P,Pol,Q,R) REM544_PENTANOM(P,Pol,K1,K2,K3,KI,KI64,\
                                                  K164,K264,K364,Q,R,HFE_MASKn)
        #endif
    #else
        #define hfemul mul576
        #define hfesquare square576

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM576_TRINOM(P,Pol,K3,KI,KI64,K364,\
                                                    Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #if (HFEnr)
                #define HFEREM(P,Pol,Q,R) REM576_PENTANOM(P,Pol,K1,K2,K3,KI,\
                                             KI64,K164,K264,K364,Q,R,HFE_MASKn)
            #else
                /* HFEn == 576 */
                #define HFEREM(P,Pol,Q,R) REM576_PENTANOM_K576(P,Pol,K1,K2,K3,\
                                                               K164,K264,K364,R)
            #endif
        #endif
    #endif

#elif (NB_WORD_GFqn == 10)
    #define HFEADD2 ADD640_2

    #define HFECOPY COPY640
    #define HFEPERMUTATION PERMUTATION640
    #define HFESET0 SET0_640

    #define HFEISZERO ISZERO640
    #define HFECMP_LT CMP_LT640
    #define HFECMP_GT CMP_GT640
    #define HFEDOTPRODUCT DOTPRODUCT640
    #define HFEDOTPRODUCTN DOTPRODUCT640M

    #if (HFEn < 609)
        #define hfemul mul608
        #define hfesquare square608

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM608_TRINOM(P,Pol,K3,KI,KI64,K364,\
                                                    Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #define HFEREM(P,Pol,Q,R) REM608_PENTANOM(P,Pol,K1,K2,K3,KI,KI64,\
                                                  K164,K264,K364,Q,R,HFE_MASKn)
        #endif
    #else
        #define hfemul mul640
        #define hfesquare square640

        #ifdef __TRINOMHFE__
            #define HFEREM(P,Pol,Q,R) REM640_TRINOM(P,Pol,K3,KI,KI64,K364,\
                                                    Q,R,HFE_MASKn)
        #endif

        #ifdef __PENTANOMHFE__
            #if (HFEnr)
                #define HFEREM(P,Pol,Q,R) REM640_PENTANOM(P,Pol,K1,K2,K3,KI,\
                                             KI64,K164,K264,K364,Q,R,HFE_MASKn)
            #else
                /* HFEn == 640 */
                #define HFEREM(P,Pol,Q,R) REM640_PENTANOM_K640(P,Pol,K1,K2,K3,\
                                                               K164,K264,K364,R)
            #endif
        #endif
    #endif
#endif

#if (NB_WORD_GFqm == 1)
    #define HFEDOTPRODUCTM DOTPRODUCT64M
    #define HFECOPY_M COPY64
    #define HFEISEQUALm ISEQUAL64
#elif (NB_WORD_GFqm == 2)
    #define HFEDOTPRODUCTM DOTPRODUCT128M
    #define HFECOPY_M COPY128
    #define HFEISEQUALm ISEQUAL128
#elif (NB_WORD_GFqm == 3)
    #define HFEDOTPRODUCTM DOTPRODUCT192M
    #define HFECOPY_M COPY192
    #define HFEISEQUALm ISEQUAL192
#elif (NB_WORD_GFqm == 4)
    #define HFEDOTPRODUCTM DOTPRODUCT256M
    #define HFECOPY_M COPY256
    #define HFEISEQUALm ISEQUAL256
#elif (NB_WORD_GFqm == 5)
    #define HFEDOTPRODUCTM DOTPRODUCT320M
    #define HFECOPY_M COPY320
    #define HFEISEQUALm ISEQUAL320
#elif (NB_WORD_GFqm == 6)
    #define HFEDOTPRODUCTM DOTPRODUCT384M
    #define HFECOPY_M COPY384
    #define HFEISEQUALm ISEQUAL384
#elif (NB_WORD_GFqm == 7)
    #define HFEDOTPRODUCTM DOTPRODUCT448M
    #define HFECOPY_M COPY448
    #define HFEISEQUALm ISEQUAL448
#elif (NB_WORD_GFqm == 8)
    #define HFEDOTPRODUCTM DOTPRODUCT512M
    #define HFECOPY_M COPY512
    #define HFEISEQUALm ISEQUAL512
#elif (NB_WORD_GFqm == 9)
    #define HFEDOTPRODUCTM DOTPRODUCT576M
    #define HFECOPY_M COPY576
    #define HFEISEQUALm ISEQUAL576
#elif (NB_WORD_GFqm == 10)
    #define HFEDOTPRODUCTM DOTPRODUCT640M
    #define HFECOPY_M COPY640
    #define HFEISEQUALm ISEQUAL640
#endif

#endif
