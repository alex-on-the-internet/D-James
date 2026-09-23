#ifndef _REM_H
#define _REM_H

#include <stdint.h>
#include "add.h"


/* Modular reduction in GF(2^k) on 64-bit words, by a trinomial
   x^k + x^k3 + 1 or a pentanomial x^k + x^k3 + x^k2 + x^k1 + 1.

   Requirements of every macro:
    Degree(Pol) <= 2*(k-1)
    k3 < sup(k/2)+1
    k3 <= 32

   Two reductions are used, so the leading term x^(2*(k-1)) becomes
   x^(2*(k3-1)) at the end; the result is reduced if 2*(k3-1) <= k-1, i.e.
   k3 < sup(k/2)+1. The last remainder fits in one 64-bit word, which also
   needs 2*(k3-1) <= 63, i.e. k3 <= 32.

   The variable mask is mask64(k&63) (see bit.h). */


/* P is the result, Pol is the input. Q and R are temporaries. */


#define MACRO_128_TRINOM(ROW1,ROW2,P,Pol,k3,ki,Q,R,mask) \
    /* x^k = x^k3 + 1 */\
    ROW1;\
    ROW2;\
    /* x^k = 1 */\
    ADD128(P,Pol,Q);\
    /* x^k = x^k3 */\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>(64-k3))^(Q[1]<<k3);\
    /* x^k = x^k3 + 1 */\
    R=(ki>=k3)?Q[1]>>(ki-k3):(Q[0]>>(64-(k3-ki)))^(Q[1]<<(k3-ki));\
    /* x^k = 1 */\
    P[0]^=R;\
    /* x^k = x^k3 */\
    P[0]^=R<<k3;\
    P[1]&=mask;

#define REM96_TRINOM(P,Pol,k3,ki,Q,R,mask) \
    MACRO_128_TRINOM(Q[0]=(Pol[1]>>ki)^(Pol[2]<<(64-ki)),Q[1]=Pol[2]>>ki,P,Pol,k3,ki,Q,R,mask)

#define REM128_TRINOM(P,Pol,k3,ki,ki64,Q,R,mask) \
    MACRO_128_TRINOM(Q[0]=(Pol[1]>>ki)^(Pol[2]<<(ki64)),Q[1]=(Pol[2]>>ki)^(Pol[3]<<ki64),P,Pol,k3,ki,Q,R,mask)


#define MACRO_128_PENTANOM(ROW1,ROW2,P,Pol,k1,k2,k3,ki,ki64,Q,R,mask) \
    /* x^k = x^k3 + x^k2 + x^k1 + 1 */\
    ROW1;\
    ROW2;\
    /* x^k = 1 */\
    ADD128(P,Pol,Q);\
    /* x^k = x^k1 */\
    P[0]^=Q[0]<<k1;\
    P[1]^=(Q[0]>>(64-k1))^(Q[1]<<k1);\
    /* x^k = x^k2 */\
    P[0]^=Q[0]<<k2;\
    P[1]^=(Q[0]>>(64-k2))^(Q[1]<<k2);\
    /* x^k = x^k3 */\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>(64-k3))^(Q[1]<<k3);\
    /* x^k = x^k3 + x^k2 + x^k1 + 1 */\
    R=(ki>=k3)?Q[1]>>(ki-k3):(Q[0]>>(64-(k3-ki)))^(Q[1]<<(k3-ki));\
    R^=(ki>=k2)?Q[1]>>(ki-k2):(Q[0]>>(64-(k2-ki)))^(Q[1]<<(k2-ki));\
    R^=(ki>=k1)?Q[1]>>(ki-k1):(Q[0]>>(64-(k1-ki)))^(Q[1]<<(k1-ki));\
    /* x^k = 1 */\
    P[0]^=R;\
    /* x^k = x^k1 */\
    P[0]^=R<<k1;\
    /* x^k = x^k2 */\
    P[0]^=R<<k2;\
    /* x^k = x^k3 */\
    P[0]^=R<<k3;\
    P[1]&=mask;

#define REM96_PENTANOM(P,Pol,k1,k2,k3,ki,Q,R,mask) \
    MACRO_128_PENTANOM(Q[0]=(Pol[1]>>ki)^(Pol[2]<<(64-ki)),Q[1]=(Pol[2]>>ki),P,Pol,k1,k2,k3,ki,,Q,R,mask)

#define REM128_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,Q,R,mask) \
    MACRO_128_PENTANOM(Q[0]=(Pol[1]>>ki)^(Pol[2]<<ki64),Q[1]=(Pol[2]>>ki)^(Pol[3]<<ki64),P,Pol,k1,k2,k3,ki,ki64,Q,R,mask)


/* Special case k==128 */
#define REM128_PENTANOM_K128(P,Pol,k1,k2,k3,R) \
    ADD128(P,Pol,Pol+2);\
    P[0]^=Pol[2]<<k1;\
    P[1]^=(Pol[2]>>(64-k1))^(Pol[3]<<k1);\
    P[0]^=Pol[2]<<k2;\
    P[1]^=(Pol[2]>>(64-k2))^(Pol[3]<<k2);\
    P[0]^=Pol[2]<<k3;\
    P[1]^=(Pol[2]>>(64-k3))^(Pol[3]<<k3);\
    R=Pol[3]>>(64-k3);\
    R^=Pol[3]>>(64-k2);\
    /* Useless if k1==1 */\
    R^=Pol[3]>>(64-k1);\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;


#define MACRO_192_TRINOM(ROW,P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    Q[0]=(Pol[2]>>ki)^(Pol[3]<<ki64);\
    Q[1]=(Pol[3]>>ki)^(Pol[4]<<ki64);\
    ROW;\
    ADD192(P,Pol,Q);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    /* 64-(k364+ki) == (k3-ki) */\
    R=(ki>=k3)?Q[2]>>(ki-k3):(Q[1]>>(k364+ki))^(Q[2]<<(k3-ki));\
    P[0]^=R;\
    P[0]^=R<<k3;\
    P[2]&=mask;

#define REM160_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_192_TRINOM(Q[2]=Pol[4]>>ki,P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define REM192_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_192_TRINOM(Q[2]=(Pol[4]>>ki)^(Pol[5]<<ki64),P,Pol,k3,ki,ki64,k364,Q,R,mask)


#define MACRO_192_PENTANOM(ROW,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    Q[0]=(Pol[2]>>ki)^(Pol[3]<<ki64);\
    Q[1]=(Pol[3]>>ki)^(Pol[4]<<ki64);\
    ROW;\
    ADD192(P,Pol,Q);\
    P[0]^=Q[0]<<k1;\
    P[1]^=(Q[0]>>k164)^(Q[1]<<k1);\
    P[2]^=(Q[1]>>k164)^(Q[2]<<k1);\
    P[0]^=Q[0]<<k2;\
    P[1]^=(Q[0]>>k264)^(Q[1]<<k2);\
    P[2]^=(Q[1]>>k264)^(Q[2]<<k2);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    /* 64-(k364+ki) == (k3-ki) */\
    R=(ki>=k3)?Q[2]>>(ki-k3):(Q[1]>>(k364+ki))^(Q[2]<<(k3-ki));\
    R^=(ki>=k2)?Q[2]>>(ki-k2):(Q[1]>>(k264+ki))^(Q[2]<<(k2-ki));\
    R^=(ki>=k1)?Q[2]>>(ki-k1):(Q[1]>>(k164+ki))^(Q[2]<<(k1-ki));\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;\
    P[2]&=mask;

#define REM160_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_192_PENTANOM(Q[2]=(Pol[4]>>ki),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)

#define REM192_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_192_PENTANOM(Q[2]=(Pol[4]>>ki)^(Pol[5]<<ki64),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)


/* Special case k==192 */
#define REM192_PENTANOM_K192(P,Pol,k1,k2,k3,k164,k264,k364,R) \
    ADD192(P,Pol,Pol+3);\
    P[0]^=Pol[3]<<k1;\
    P[1]^=(Pol[3]>>k164)^(Pol[4]<<k1);\
    P[2]^=(Pol[4]>>k164)^(Pol[5]<<k1);\
    P[0]^=Pol[3]<<k2;\
    P[1]^=(Pol[3]>>k264)^(Pol[4]<<k2);\
    P[2]^=(Pol[4]>>k264)^(Pol[5]<<k2);\
    P[0]^=Pol[3]<<k3;\
    P[1]^=(Pol[3]>>k364)^(Pol[4]<<k3);\
    P[2]^=(Pol[4]>>k364)^(Pol[5]<<k3);\
    R=Pol[5]>>k364;\
    R^=Pol[5]>>k264;\
    /* Useless if k1==1 */\
    R^=Pol[5]>>k164;\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;


#define MACRO_256_TRINOM(ROW,P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    Q[0]=(Pol[3]>>ki)^(Pol[4]<<ki64);\
    Q[1]=(Pol[4]>>ki)^(Pol[5]<<ki64);\
    Q[2]=(Pol[5]>>ki)^(Pol[6]<<ki64);\
    ROW;\
    ADD256(P,Pol,Q);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    /* 64-(k364+ki) == (k3-ki) */\
    R=(ki>=k3)?Q[3]>>(ki-k3):(Q[2]>>(k364+ki))^(Q[3]<<(k3-ki));\
    P[0]^=R;\
    P[0]^=R<<k3;\
    P[3]&=mask;

#define REM224_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_256_TRINOM(Q[3]=Pol[6]>>ki,P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define REM256_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_256_TRINOM(Q[3]=(Pol[6]>>ki)^(Pol[7]<<ki64),P,Pol,k3,ki,ki64,k364,Q,R,mask)


#define MACRO_256_PENTANOM(ROW,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    Q[0]=(Pol[3]>>ki)^(Pol[4]<<ki64);\
    Q[1]=(Pol[4]>>ki)^(Pol[5]<<ki64);\
    Q[2]=(Pol[5]>>ki)^(Pol[6]<<ki64);\
    ROW;\
    ADD256(P,Pol,Q);\
    P[0]^=Q[0]<<k1;\
    P[1]^=(Q[0]>>k164)^(Q[1]<<k1);\
    P[2]^=(Q[1]>>k164)^(Q[2]<<k1);\
    P[3]^=(Q[2]>>k164)^(Q[3]<<k1);\
    P[0]^=Q[0]<<k2;\
    P[1]^=(Q[0]>>k264)^(Q[1]<<k2);\
    P[2]^=(Q[1]>>k264)^(Q[2]<<k2);\
    P[3]^=(Q[2]>>k264)^(Q[3]<<k2);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    /* 64-(k364+ki) == (k3-ki) */\
    R=(ki>=k3)?Q[3]>>(ki-k3):(Q[2]>>(k364+ki))^(Q[3]<<(k3-ki));\
    R^=(ki>=k2)?Q[3]>>(ki-k2):(Q[2]>>(k264+ki))^(Q[3]<<(k2-ki));\
    R^=(ki>=k1)?Q[3]>>(ki-k1):(Q[2]>>(k164+ki))^(Q[3]<<(k1-ki));\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;\
    P[3]&=mask;

#define REM224_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_256_PENTANOM(Q[3]=(Pol[6]>>ki),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)

#define REM256_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_256_PENTANOM(Q[3]=(Pol[6]>>ki)^(Pol[7]<<ki64),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)


/* Special case k==256 */
#define REM256_PENTANOM_K256(P,Pol,k1,k2,k3,k164,k264,k364,R) \
    ADD256(P,Pol,Pol+4);\
    P[0]^=Pol[4]<<k1;\
    P[1]^=(Pol[4]>>k164)^(Pol[5]<<k1);\
    P[2]^=(Pol[5]>>k164)^(Pol[6]<<k1);\
    P[3]^=(Pol[6]>>k164)^(Pol[7]<<k1);\
    P[0]^=Pol[4]<<k2;\
    P[1]^=(Pol[4]>>k264)^(Pol[5]<<k2);\
    P[2]^=(Pol[5]>>k264)^(Pol[6]<<k2);\
    P[3]^=(Pol[6]>>k264)^(Pol[7]<<k2);\
    P[0]^=Pol[4]<<k3;\
    P[1]^=(Pol[4]>>k364)^(Pol[5]<<k3);\
    P[2]^=(Pol[5]>>k364)^(Pol[6]<<k3);\
    P[3]^=(Pol[6]>>k364)^(Pol[7]<<k3);\
    R=Pol[7]>>k364;\
    R^=Pol[7]>>k264;\
    /* Useless if k1==1 */\
    R^=Pol[7]>>k164;\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;


#define MACRO_320_TRINOM(ROW,P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    Q[0]=(Pol[4]>>ki)^(Pol[5]<<ki64);\
    Q[1]=(Pol[5]>>ki)^(Pol[6]<<ki64);\
    Q[2]=(Pol[6]>>ki)^(Pol[7]<<ki64);\
    Q[3]=(Pol[7]>>ki)^(Pol[8]<<ki64);\
    ROW;\
    ADD320(P,Pol,Q);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    /* 64-(k364+ki) == (k3-ki) */\
    R=(ki>=k3)?Q[4]>>(ki-k3):(Q[3]>>(k364+ki))^(Q[4]<<(k3-ki));\
    P[0]^=R;\
    P[0]^=R<<k3;\
    P[4]&=mask;

#define REM288_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_320_TRINOM(Q[4]=(Pol[8]>>ki),P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define REM320_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_320_TRINOM(Q[4]=(Pol[8]>>ki)^(Pol[9]<<ki64),P,Pol,k3,ki,ki64,k364,Q,R,mask)


#define MACRO_320_PENTANOM(ROW,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    Q[0]=(Pol[4]>>ki)^(Pol[5]<<ki64);\
    Q[1]=(Pol[5]>>ki)^(Pol[6]<<ki64);\
    Q[2]=(Pol[6]>>ki)^(Pol[7]<<ki64);\
    Q[3]=(Pol[7]>>ki)^(Pol[8]<<ki64);\
    ROW;\
    ADD320(P,Pol,Q);\
    P[0]^=Q[0]<<k1;\
    P[1]^=(Q[0]>>k164)^(Q[1]<<k1);\
    P[2]^=(Q[1]>>k164)^(Q[2]<<k1);\
    P[3]^=(Q[2]>>k164)^(Q[3]<<k1);\
    P[4]^=(Q[3]>>k164)^(Q[4]<<k1);\
    P[0]^=Q[0]<<k2;\
    P[1]^=(Q[0]>>k264)^(Q[1]<<k2);\
    P[2]^=(Q[1]>>k264)^(Q[2]<<k2);\
    P[3]^=(Q[2]>>k264)^(Q[3]<<k2);\
    P[4]^=(Q[3]>>k264)^(Q[4]<<k2);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    /* 64-(k364+ki) == (k3-ki) */\
    R=(ki>=k3)?Q[4]>>(ki-k3):(Q[3]>>(k364+ki))^(Q[4]<<(k3-ki));\
    R^=(ki>=k2)?Q[4]>>(ki-k2):(Q[3]>>(k264+ki))^(Q[4]<<(k2-ki));\
    R^=(ki>=k1)?Q[4]>>(ki-k1):(Q[3]>>(k164+ki))^(Q[4]<<(k1-ki));\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;\
    P[4]&=mask;

#define REM288_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_320_PENTANOM(Q[4]=(Pol[8]>>ki),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)

#define REM320_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_320_PENTANOM(Q[4]=(Pol[8]>>ki)^(Pol[9]<<ki64),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)


/* Special case k==320 */
#define REM320_PENTANOM_K320(P,Pol,k1,k2,k3,k164,k264,k364,R) \
    ADD320(P,Pol,Pol+5);\
    P[0]^=Pol[5]<<k1;\
    P[1]^=(Pol[5]>>k164)^(Pol[6]<<k1);\
    P[2]^=(Pol[6]>>k164)^(Pol[7]<<k1);\
    P[3]^=(Pol[7]>>k164)^(Pol[8]<<k1);\
    P[4]^=(Pol[8]>>k164)^(Pol[9]<<k1);\
    P[0]^=Pol[5]<<k2;\
    P[1]^=(Pol[5]>>k264)^(Pol[6]<<k2);\
    P[2]^=(Pol[6]>>k264)^(Pol[7]<<k2);\
    P[3]^=(Pol[7]>>k264)^(Pol[8]<<k2);\
    P[4]^=(Pol[8]>>k264)^(Pol[9]<<k2);\
    P[0]^=Pol[5]<<k3;\
    P[1]^=(Pol[5]>>k364)^(Pol[6]<<k3);\
    P[2]^=(Pol[6]>>k364)^(Pol[7]<<k3);\
    P[3]^=(Pol[7]>>k364)^(Pol[8]<<k3);\
    P[4]^=(Pol[8]>>k364)^(Pol[9]<<k3);\
    R=Pol[9]>>k364;\
    R^=Pol[9]>>k264;\
    /* Useless if k1==1 */\
    R^=Pol[9]>>k164;\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;


#define MACRO_384_TRINOM(ROW,P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    Q[0]=(Pol[5]>>ki)^(Pol[6]<<ki64);\
    Q[1]=(Pol[6]>>ki)^(Pol[7]<<ki64);\
    Q[2]=(Pol[7]>>ki)^(Pol[8]<<ki64);\
    Q[3]=(Pol[8]>>ki)^(Pol[9]<<ki64);\
    Q[4]=(Pol[9]>>ki)^(Pol[10]<<ki64);\
    ROW;\
    ADD384(P,Pol,Q);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    P[5]^=(Q[4]>>k364)^(Q[5]<<k3);\
    /* 64-(k364+ki) == (k3-ki) */\
    R=(ki>=k3)?Q[5]>>(ki-k3):(Q[4]>>(k364+ki))^(Q[5]<<(k3-ki));\
    P[0]^=R;\
    P[0]^=R<<k3;\
    P[5]&=mask;

#define REM352_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_384_TRINOM(Q[5]=(Pol[10]>>ki),P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define REM384_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_384_TRINOM(Q[5]=(Pol[10]>>ki)^(Pol[11]<<ki64),P,Pol,k3,ki,ki64,k364,Q,R,mask)


#define MACRO_384_PENTANOM(ROW,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    Q[0]=(Pol[5]>>ki)^(Pol[6]<<ki64);\
    Q[1]=(Pol[6]>>ki)^(Pol[7]<<ki64);\
    Q[2]=(Pol[7]>>ki)^(Pol[8]<<ki64);\
    Q[3]=(Pol[8]>>ki)^(Pol[9]<<ki64);\
    Q[4]=(Pol[9]>>ki)^(Pol[10]<<ki64);\
    ROW;\
    ADD384(P,Pol,Q);\
    P[0]^=Q[0]<<k1;\
    P[1]^=(Q[0]>>k164)^(Q[1]<<k1);\
    P[2]^=(Q[1]>>k164)^(Q[2]<<k1);\
    P[3]^=(Q[2]>>k164)^(Q[3]<<k1);\
    P[4]^=(Q[3]>>k164)^(Q[4]<<k1);\
    P[5]^=(Q[4]>>k164)^(Q[5]<<k1);\
    P[0]^=Q[0]<<k2;\
    P[1]^=(Q[0]>>k264)^(Q[1]<<k2);\
    P[2]^=(Q[1]>>k264)^(Q[2]<<k2);\
    P[3]^=(Q[2]>>k264)^(Q[3]<<k2);\
    P[4]^=(Q[3]>>k264)^(Q[4]<<k2);\
    P[5]^=(Q[4]>>k264)^(Q[5]<<k2);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    P[5]^=(Q[4]>>k364)^(Q[5]<<k3);\
    /* 64-(k364+ki) == (k3-ki) */\
    R=(ki>=k3)?Q[5]>>(ki-k3):(Q[4]>>(k364+ki))^(Q[5]<<(k3-ki));\
    R^=(ki>=k2)?Q[5]>>(ki-k2):(Q[4]>>(k264+ki))^(Q[5]<<(k2-ki));\
    R^=(ki>=k1)?Q[5]>>(ki-k1):(Q[4]>>(k164+ki))^(Q[5]<<(k1-ki));\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;\
    P[5]&=mask;

#define REM352_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_384_PENTANOM(Q[5]=(Pol[10]>>ki),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)

#define REM384_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_384_PENTANOM(Q[5]=(Pol[10]>>ki)^(Pol[11]<<ki64),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)


/* Special case k==384 */
#define REM384_PENTANOM_K384(P,Pol,k1,k2,k3,k164,k264,k364,R) \
    ADD384(P,Pol,Pol+6);\
    P[0]^=Pol[6]<<k1;\
    P[1]^=(Pol[6]>>k164)^(Pol[7]<<k1);\
    P[2]^=(Pol[7]>>k164)^(Pol[8]<<k1);\
    P[3]^=(Pol[8]>>k164)^(Pol[9]<<k1);\
    P[4]^=(Pol[9]>>k164)^(Pol[10]<<k1);\
    P[5]^=(Pol[10]>>k164)^(Pol[11]<<k1);\
    P[0]^=Pol[6]<<k2;\
    P[1]^=(Pol[6]>>k264)^(Pol[7]<<k2);\
    P[2]^=(Pol[7]>>k264)^(Pol[8]<<k2);\
    P[3]^=(Pol[8]>>k264)^(Pol[9]<<k2);\
    P[4]^=(Pol[9]>>k264)^(Pol[10]<<k2);\
    P[5]^=(Pol[10]>>k264)^(Pol[11]<<k2);\
    P[0]^=Pol[6]<<k3;\
    P[1]^=(Pol[6]>>k364)^(Pol[7]<<k3);\
    P[2]^=(Pol[7]>>k364)^(Pol[8]<<k3);\
    P[3]^=(Pol[8]>>k364)^(Pol[9]<<k3);\
    P[4]^=(Pol[9]>>k364)^(Pol[10]<<k3);\
    P[5]^=(Pol[10]>>k364)^(Pol[11]<<k3);\
    R=Pol[11]>>k364;\
    R^=Pol[11]>>k264;\
    /* Useless if k1==1 */\
    R^=Pol[11]>>k164;\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;


#define MACRO_448_TRINOM(ROW,P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    Q[0]=(Pol[6]>>ki)^(Pol[7]<<ki64);\
    Q[1]=(Pol[7]>>ki)^(Pol[8]<<ki64);\
    Q[2]=(Pol[8]>>ki)^(Pol[9]<<ki64);\
    Q[3]=(Pol[9]>>ki)^(Pol[10]<<ki64);\
    Q[4]=(Pol[10]>>ki)^(Pol[11]<<ki64);\
    Q[5]=(Pol[11]>>ki)^(Pol[12]<<ki64);\
    ROW;\
    ADD448(P,Pol,Q);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    P[5]^=(Q[4]>>k364)^(Q[5]<<k3);\
    P[6]^=(Q[5]>>k364)^(Q[6]<<k3);\
    R=(ki>=k3)?Q[6]>>(ki-k3):(Q[5]>>(k364+ki))^(Q[6]<<(k3-ki));\
    P[0]^=R;\
    P[0]^=R<<k3;\
    P[6]&=mask;

#define REM416_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_448_TRINOM(Q[6]=Pol[12]>>ki,P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define REM448_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_448_TRINOM(Q[6]=(Pol[12]>>ki)^(Pol[13]<<ki64),P,Pol,k3,ki,ki64,k364,Q,R,mask)


#define MACRO_448_PENTANOM(ROW,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    Q[0]=(Pol[6]>>ki)^(Pol[7]<<ki64);\
    Q[1]=(Pol[7]>>ki)^(Pol[8]<<ki64);\
    Q[2]=(Pol[8]>>ki)^(Pol[9]<<ki64);\
    Q[3]=(Pol[9]>>ki)^(Pol[10]<<ki64);\
    Q[4]=(Pol[10]>>ki)^(Pol[11]<<ki64);\
    Q[5]=(Pol[11]>>ki)^(Pol[12]<<ki64);\
    ROW;\
    ADD448(P,Pol,Q);\
    P[0]^=Q[0]<<k1;\
    P[1]^=(Q[0]>>k164)^(Q[1]<<k1);\
    P[2]^=(Q[1]>>k164)^(Q[2]<<k1);\
    P[3]^=(Q[2]>>k164)^(Q[3]<<k1);\
    P[4]^=(Q[3]>>k164)^(Q[4]<<k1);\
    P[5]^=(Q[4]>>k164)^(Q[5]<<k1);\
    P[6]^=(Q[5]>>k164)^(Q[6]<<k1);\
    P[0]^=Q[0]<<k2;\
    P[1]^=(Q[0]>>k264)^(Q[1]<<k2);\
    P[2]^=(Q[1]>>k264)^(Q[2]<<k2);\
    P[3]^=(Q[2]>>k264)^(Q[3]<<k2);\
    P[4]^=(Q[3]>>k264)^(Q[4]<<k2);\
    P[5]^=(Q[4]>>k264)^(Q[5]<<k2);\
    P[6]^=(Q[5]>>k264)^(Q[6]<<k2);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    P[5]^=(Q[4]>>k364)^(Q[5]<<k3);\
    P[6]^=(Q[5]>>k364)^(Q[6]<<k3);\
    R=(ki>=k3)?Q[6]>>(ki-k3):(Q[5]>>(k364+ki))^(Q[6]<<(k3-ki));\
    R^=(ki>=k2)?Q[6]>>(ki-k2):(Q[5]>>(k264+ki))^(Q[6]<<(k2-ki));\
    R^=(ki>=k1)?Q[6]>>(ki-k1):(Q[5]>>(k164+ki))^(Q[6]<<(k1-ki));\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;\
    P[6]&=mask;

#define REM416_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_448_PENTANOM(Q[6]=Pol[12]>>ki,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)

#define REM448_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_448_PENTANOM(Q[6]=(Pol[12]>>ki)^(Pol[13]<<ki64),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)


/* Special case k==448 */
#define REM448_PENTANOM_K448(P,Pol,k1,k2,k3,k164,k264,k364,R) \
    ADD448(P,Pol,Pol+7);\
    P[0]^=Pol[7]<<k1;\
    P[1]^=(Pol[7]>>k164)^(Pol[8]<<k1);\
    P[2]^=(Pol[8]>>k164)^(Pol[9]<<k1);\
    P[3]^=(Pol[9]>>k164)^(Pol[10]<<k1);\
    P[4]^=(Pol[10]>>k164)^(Pol[11]<<k1);\
    P[5]^=(Pol[11]>>k164)^(Pol[12]<<k1);\
    P[6]^=(Pol[12]>>k164)^(Pol[13]<<k1);\
    P[0]^=Pol[7]<<k2;\
    P[1]^=(Pol[7]>>k264)^(Pol[8]<<k2);\
    P[2]^=(Pol[8]>>k264)^(Pol[9]<<k2);\
    P[3]^=(Pol[9]>>k264)^(Pol[10]<<k2);\
    P[4]^=(Pol[10]>>k264)^(Pol[11]<<k2);\
    P[5]^=(Pol[11]>>k264)^(Pol[12]<<k2);\
    P[6]^=(Pol[12]>>k264)^(Pol[13]<<k2);\
    P[0]^=Pol[7]<<k3;\
    P[1]^=(Pol[7]>>k364)^(Pol[8]<<k3);\
    P[2]^=(Pol[8]>>k364)^(Pol[9]<<k3);\
    P[3]^=(Pol[9]>>k364)^(Pol[10]<<k3);\
    P[4]^=(Pol[10]>>k364)^(Pol[11]<<k3);\
    P[5]^=(Pol[11]>>k364)^(Pol[12]<<k3);\
    P[6]^=(Pol[12]>>k364)^(Pol[13]<<k3);\
    R=Pol[13]>>k364;\
    R^=Pol[13]>>k264;\
    R^=Pol[13]>>k164;\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;

#define MACRO_512_TRINOM(ROW,P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    Q[0]=(Pol[7]>>ki)^(Pol[8]<<ki64);\
    Q[1]=(Pol[8]>>ki)^(Pol[9]<<ki64);\
    Q[2]=(Pol[9]>>ki)^(Pol[10]<<ki64);\
    Q[3]=(Pol[10]>>ki)^(Pol[11]<<ki64);\
    Q[4]=(Pol[11]>>ki)^(Pol[12]<<ki64);\
    Q[5]=(Pol[12]>>ki)^(Pol[13]<<ki64);\
    Q[6]=(Pol[13]>>ki)^(Pol[14]<<ki64);\
    ROW;\
    ADD512(P,Pol,Q);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    P[5]^=(Q[4]>>k364)^(Q[5]<<k3);\
    P[6]^=(Q[5]>>k364)^(Q[6]<<k3);\
    P[7]^=(Q[6]>>k364)^(Q[7]<<k3);\
    R=(ki>=k3)?Q[7]>>(ki-k3):(Q[6]>>(k364+ki))^(Q[7]<<(k3-ki));\
    P[0]^=R;\
    P[0]^=R<<k3;\
    P[7]&=mask;

#define REM480_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_512_TRINOM(Q[7]=Pol[14]>>ki,P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define REM512_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_512_TRINOM(Q[7]=(Pol[14]>>ki)^(Pol[15]<<ki64),P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define MACRO_512_PENTANOM(ROW,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    Q[0]=(Pol[7]>>ki)^(Pol[8]<<ki64);\
    Q[1]=(Pol[8]>>ki)^(Pol[9]<<ki64);\
    Q[2]=(Pol[9]>>ki)^(Pol[10]<<ki64);\
    Q[3]=(Pol[10]>>ki)^(Pol[11]<<ki64);\
    Q[4]=(Pol[11]>>ki)^(Pol[12]<<ki64);\
    Q[5]=(Pol[12]>>ki)^(Pol[13]<<ki64);\
    Q[6]=(Pol[13]>>ki)^(Pol[14]<<ki64);\
    ROW;\
    ADD512(P,Pol,Q);\
    P[0]^=Q[0]<<k1;\
    P[1]^=(Q[0]>>k164)^(Q[1]<<k1);\
    P[2]^=(Q[1]>>k164)^(Q[2]<<k1);\
    P[3]^=(Q[2]>>k164)^(Q[3]<<k1);\
    P[4]^=(Q[3]>>k164)^(Q[4]<<k1);\
    P[5]^=(Q[4]>>k164)^(Q[5]<<k1);\
    P[6]^=(Q[5]>>k164)^(Q[6]<<k1);\
    P[7]^=(Q[6]>>k164)^(Q[7]<<k1);\
    P[0]^=Q[0]<<k2;\
    P[1]^=(Q[0]>>k264)^(Q[1]<<k2);\
    P[2]^=(Q[1]>>k264)^(Q[2]<<k2);\
    P[3]^=(Q[2]>>k264)^(Q[3]<<k2);\
    P[4]^=(Q[3]>>k264)^(Q[4]<<k2);\
    P[5]^=(Q[4]>>k264)^(Q[5]<<k2);\
    P[6]^=(Q[5]>>k264)^(Q[6]<<k2);\
    P[7]^=(Q[6]>>k264)^(Q[7]<<k2);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    P[5]^=(Q[4]>>k364)^(Q[5]<<k3);\
    P[6]^=(Q[5]>>k364)^(Q[6]<<k3);\
    P[7]^=(Q[6]>>k364)^(Q[7]<<k3);\
    R=(ki>=k3)?Q[7]>>(ki-k3):(Q[6]>>(k364+ki))^(Q[7]<<(k3-ki));\
    R^=(ki>=k2)?Q[7]>>(ki-k2):(Q[6]>>(k264+ki))^(Q[7]<<(k2-ki));\
    R^=(ki>=k1)?Q[7]>>(ki-k1):(Q[6]>>(k164+ki))^(Q[7]<<(k1-ki));\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;\
    P[7]&=mask;

#define REM480_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_512_PENTANOM(Q[7]=Pol[14]>>ki,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)

#define REM512_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_512_PENTANOM(Q[7]=(Pol[14]>>ki)^(Pol[15]<<ki64),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)


/* Special case k==512 */
#define REM512_PENTANOM_K512(P,Pol,k1,k2,k3,k164,k264,k364,R) \
    ADD512(P,Pol,Pol+8);\
    P[0]^=Pol[8]<<k1;\
    P[1]^=(Pol[8]>>k164)^(Pol[9]<<k1);\
    P[2]^=(Pol[9]>>k164)^(Pol[10]<<k1);\
    P[3]^=(Pol[10]>>k164)^(Pol[11]<<k1);\
    P[4]^=(Pol[11]>>k164)^(Pol[12]<<k1);\
    P[5]^=(Pol[12]>>k164)^(Pol[13]<<k1);\
    P[6]^=(Pol[13]>>k164)^(Pol[14]<<k1);\
    P[7]^=(Pol[14]>>k164)^(Pol[15]<<k1);\
    P[0]^=Pol[8]<<k2;\
    P[1]^=(Pol[8]>>k264)^(Pol[9]<<k2);\
    P[2]^=(Pol[9]>>k264)^(Pol[10]<<k2);\
    P[3]^=(Pol[10]>>k264)^(Pol[11]<<k2);\
    P[4]^=(Pol[11]>>k264)^(Pol[12]<<k2);\
    P[5]^=(Pol[12]>>k264)^(Pol[13]<<k2);\
    P[6]^=(Pol[13]>>k264)^(Pol[14]<<k2);\
    P[7]^=(Pol[14]>>k264)^(Pol[15]<<k2);\
    P[0]^=Pol[8]<<k3;\
    P[1]^=(Pol[8]>>k364)^(Pol[9]<<k3);\
    P[2]^=(Pol[9]>>k364)^(Pol[10]<<k3);\
    P[3]^=(Pol[10]>>k364)^(Pol[11]<<k3);\
    P[4]^=(Pol[11]>>k364)^(Pol[12]<<k3);\
    P[5]^=(Pol[12]>>k364)^(Pol[13]<<k3);\
    P[6]^=(Pol[13]>>k364)^(Pol[14]<<k3);\
    P[7]^=(Pol[14]>>k364)^(Pol[15]<<k3);\
    R=Pol[15]>>k364;\
    R^=Pol[15]>>k264;\
    R^=Pol[15]>>k164;\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;

#define MACRO_576_TRINOM(ROW,P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    Q[0]=(Pol[8]>>ki)^(Pol[9]<<ki64);\
    Q[1]=(Pol[9]>>ki)^(Pol[10]<<ki64);\
    Q[2]=(Pol[10]>>ki)^(Pol[11]<<ki64);\
    Q[3]=(Pol[11]>>ki)^(Pol[12]<<ki64);\
    Q[4]=(Pol[12]>>ki)^(Pol[13]<<ki64);\
    Q[5]=(Pol[13]>>ki)^(Pol[14]<<ki64);\
    Q[6]=(Pol[14]>>ki)^(Pol[15]<<ki64);\
    Q[7]=(Pol[15]>>ki)^(Pol[16]<<ki64);\
    ROW;\
    ADD576(P,Pol,Q);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    P[5]^=(Q[4]>>k364)^(Q[5]<<k3);\
    P[6]^=(Q[5]>>k364)^(Q[6]<<k3);\
    P[7]^=(Q[6]>>k364)^(Q[7]<<k3);\
    P[8]^=(Q[7]>>k364)^(Q[8]<<k3);\
    R=(ki>=k3)?Q[8]>>(ki-k3):(Q[7]>>(k364+ki))^(Q[8]<<(k3-ki));\
    P[0]^=R;\
    P[0]^=R<<k3;\
    P[8]&=mask;

#define REM544_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_576_TRINOM(Q[8]=Pol[16]>>ki,P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define REM576_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_576_TRINOM(Q[8]=(Pol[16]>>ki)^(Pol[17]<<ki64),P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define MACRO_576_PENTANOM(ROW,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    Q[0]=(Pol[8]>>ki)^(Pol[9]<<ki64);\
    Q[1]=(Pol[9]>>ki)^(Pol[10]<<ki64);\
    Q[2]=(Pol[10]>>ki)^(Pol[11]<<ki64);\
    Q[3]=(Pol[11]>>ki)^(Pol[12]<<ki64);\
    Q[4]=(Pol[12]>>ki)^(Pol[13]<<ki64);\
    Q[5]=(Pol[13]>>ki)^(Pol[14]<<ki64);\
    Q[6]=(Pol[14]>>ki)^(Pol[15]<<ki64);\
    Q[7]=(Pol[15]>>ki)^(Pol[16]<<ki64);\
    ROW;\
    ADD576(P,Pol,Q);\
    P[0]^=Q[0]<<k1;\
    P[1]^=(Q[0]>>k164)^(Q[1]<<k1);\
    P[2]^=(Q[1]>>k164)^(Q[2]<<k1);\
    P[3]^=(Q[2]>>k164)^(Q[3]<<k1);\
    P[4]^=(Q[3]>>k164)^(Q[4]<<k1);\
    P[5]^=(Q[4]>>k164)^(Q[5]<<k1);\
    P[6]^=(Q[5]>>k164)^(Q[6]<<k1);\
    P[7]^=(Q[6]>>k164)^(Q[7]<<k1);\
    P[8]^=(Q[7]>>k164)^(Q[8]<<k1);\
    P[0]^=Q[0]<<k2;\
    P[1]^=(Q[0]>>k264)^(Q[1]<<k2);\
    P[2]^=(Q[1]>>k264)^(Q[2]<<k2);\
    P[3]^=(Q[2]>>k264)^(Q[3]<<k2);\
    P[4]^=(Q[3]>>k264)^(Q[4]<<k2);\
    P[5]^=(Q[4]>>k264)^(Q[5]<<k2);\
    P[6]^=(Q[5]>>k264)^(Q[6]<<k2);\
    P[7]^=(Q[6]>>k264)^(Q[7]<<k2);\
    P[8]^=(Q[7]>>k264)^(Q[8]<<k2);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    P[5]^=(Q[4]>>k364)^(Q[5]<<k3);\
    P[6]^=(Q[5]>>k364)^(Q[6]<<k3);\
    P[7]^=(Q[6]>>k364)^(Q[7]<<k3);\
    P[8]^=(Q[7]>>k364)^(Q[8]<<k3);\
    R=(ki>=k3)?Q[8]>>(ki-k3):(Q[7]>>(k364+ki))^(Q[8]<<(k3-ki));\
    R^=(ki>=k2)?Q[8]>>(ki-k2):(Q[7]>>(k264+ki))^(Q[8]<<(k2-ki));\
    R^=(ki>=k1)?Q[8]>>(ki-k1):(Q[7]>>(k164+ki))^(Q[8]<<(k1-ki));\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;\
    P[8]&=mask;

#define REM544_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_576_PENTANOM(Q[8]=Pol[16]>>ki,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)

#define REM576_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_576_PENTANOM(Q[8]=(Pol[16]>>ki)^(Pol[17]<<ki64),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)


/* Special case k==576 */
#define REM576_PENTANOM_K576(P,Pol,k1,k2,k3,k164,k264,k364,R) \
    ADD576(P,Pol,Pol+9);\
    P[0]^=Pol[9]<<k1;\
    P[1]^=(Pol[9]>>k164)^(Pol[10]<<k1);\
    P[2]^=(Pol[10]>>k164)^(Pol[11]<<k1);\
    P[3]^=(Pol[11]>>k164)^(Pol[12]<<k1);\
    P[4]^=(Pol[12]>>k164)^(Pol[13]<<k1);\
    P[5]^=(Pol[13]>>k164)^(Pol[14]<<k1);\
    P[6]^=(Pol[14]>>k164)^(Pol[15]<<k1);\
    P[7]^=(Pol[15]>>k164)^(Pol[16]<<k1);\
    P[8]^=(Pol[16]>>k164)^(Pol[17]<<k1);\
    P[0]^=Pol[9]<<k2;\
    P[1]^=(Pol[9]>>k264)^(Pol[10]<<k2);\
    P[2]^=(Pol[10]>>k264)^(Pol[11]<<k2);\
    P[3]^=(Pol[11]>>k264)^(Pol[12]<<k2);\
    P[4]^=(Pol[12]>>k264)^(Pol[13]<<k2);\
    P[5]^=(Pol[13]>>k264)^(Pol[14]<<k2);\
    P[6]^=(Pol[14]>>k264)^(Pol[15]<<k2);\
    P[7]^=(Pol[15]>>k264)^(Pol[16]<<k2);\
    P[8]^=(Pol[16]>>k264)^(Pol[17]<<k2);\
    P[0]^=Pol[9]<<k3;\
    P[1]^=(Pol[9]>>k364)^(Pol[10]<<k3);\
    P[2]^=(Pol[10]>>k364)^(Pol[11]<<k3);\
    P[3]^=(Pol[11]>>k364)^(Pol[12]<<k3);\
    P[4]^=(Pol[12]>>k364)^(Pol[13]<<k3);\
    P[5]^=(Pol[13]>>k364)^(Pol[14]<<k3);\
    P[6]^=(Pol[14]>>k364)^(Pol[15]<<k3);\
    P[7]^=(Pol[15]>>k364)^(Pol[16]<<k3);\
    P[8]^=(Pol[16]>>k364)^(Pol[17]<<k3);\
    R=Pol[17]>>k364;\
    R^=Pol[17]>>k264;\
    R^=Pol[17]>>k164;\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;

#define MACRO_640_TRINOM(ROW,P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    Q[0]=(Pol[9]>>ki)^(Pol[10]<<ki64);\
    Q[1]=(Pol[10]>>ki)^(Pol[11]<<ki64);\
    Q[2]=(Pol[11]>>ki)^(Pol[12]<<ki64);\
    Q[3]=(Pol[12]>>ki)^(Pol[13]<<ki64);\
    Q[4]=(Pol[13]>>ki)^(Pol[14]<<ki64);\
    Q[5]=(Pol[14]>>ki)^(Pol[15]<<ki64);\
    Q[6]=(Pol[15]>>ki)^(Pol[16]<<ki64);\
    Q[7]=(Pol[16]>>ki)^(Pol[17]<<ki64);\
    Q[8]=(Pol[17]>>ki)^(Pol[18]<<ki64);\
    ROW;\
    ADD640(P,Pol,Q);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    P[5]^=(Q[4]>>k364)^(Q[5]<<k3);\
    P[6]^=(Q[5]>>k364)^(Q[6]<<k3);\
    P[7]^=(Q[6]>>k364)^(Q[7]<<k3);\
    P[8]^=(Q[7]>>k364)^(Q[8]<<k3);\
    P[9]^=(Q[8]>>k364)^(Q[9]<<k3);\
    R=(ki>=k3)?Q[9]>>(ki-k3):(Q[8]>>(k364+ki))^(Q[9]<<(k3-ki));\
    P[0]^=R;\
    P[0]^=R<<k3;\
    P[9]&=mask;

#define REM608_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_640_TRINOM(Q[9]=Pol[18]>>ki,P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define REM640_TRINOM(P,Pol,k3,ki,ki64,k364,Q,R,mask) \
    MACRO_640_TRINOM(Q[9]=(Pol[18]>>ki)^(Pol[19]<<ki64),P,Pol,k3,ki,ki64,k364,Q,R,mask)

#define MACRO_640_PENTANOM(ROW,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    Q[0]=(Pol[9]>>ki)^(Pol[10]<<ki64);\
    Q[1]=(Pol[10]>>ki)^(Pol[11]<<ki64);\
    Q[2]=(Pol[11]>>ki)^(Pol[12]<<ki64);\
    Q[3]=(Pol[12]>>ki)^(Pol[13]<<ki64);\
    Q[4]=(Pol[13]>>ki)^(Pol[14]<<ki64);\
    Q[5]=(Pol[14]>>ki)^(Pol[15]<<ki64);\
    Q[6]=(Pol[15]>>ki)^(Pol[16]<<ki64);\
    Q[7]=(Pol[16]>>ki)^(Pol[17]<<ki64);\
    Q[8]=(Pol[17]>>ki)^(Pol[18]<<ki64);\
    ROW;\
    ADD640(P,Pol,Q);\
    P[0]^=Q[0]<<k1;\
    P[1]^=(Q[0]>>k164)^(Q[1]<<k1);\
    P[2]^=(Q[1]>>k164)^(Q[2]<<k1);\
    P[3]^=(Q[2]>>k164)^(Q[3]<<k1);\
    P[4]^=(Q[3]>>k164)^(Q[4]<<k1);\
    P[5]^=(Q[4]>>k164)^(Q[5]<<k1);\
    P[6]^=(Q[5]>>k164)^(Q[6]<<k1);\
    P[7]^=(Q[6]>>k164)^(Q[7]<<k1);\
    P[8]^=(Q[7]>>k164)^(Q[8]<<k1);\
    P[9]^=(Q[8]>>k164)^(Q[9]<<k1);\
    P[0]^=Q[0]<<k2;\
    P[1]^=(Q[0]>>k264)^(Q[1]<<k2);\
    P[2]^=(Q[1]>>k264)^(Q[2]<<k2);\
    P[3]^=(Q[2]>>k264)^(Q[3]<<k2);\
    P[4]^=(Q[3]>>k264)^(Q[4]<<k2);\
    P[5]^=(Q[4]>>k264)^(Q[5]<<k2);\
    P[6]^=(Q[5]>>k264)^(Q[6]<<k2);\
    P[7]^=(Q[6]>>k264)^(Q[7]<<k2);\
    P[8]^=(Q[7]>>k264)^(Q[8]<<k2);\
    P[9]^=(Q[8]>>k264)^(Q[9]<<k2);\
    P[0]^=Q[0]<<k3;\
    P[1]^=(Q[0]>>k364)^(Q[1]<<k3);\
    P[2]^=(Q[1]>>k364)^(Q[2]<<k3);\
    P[3]^=(Q[2]>>k364)^(Q[3]<<k3);\
    P[4]^=(Q[3]>>k364)^(Q[4]<<k3);\
    P[5]^=(Q[4]>>k364)^(Q[5]<<k3);\
    P[6]^=(Q[5]>>k364)^(Q[6]<<k3);\
    P[7]^=(Q[6]>>k364)^(Q[7]<<k3);\
    P[8]^=(Q[7]>>k364)^(Q[8]<<k3);\
    P[9]^=(Q[8]>>k364)^(Q[9]<<k3);\
    R=(ki>=k3)?Q[9]>>(ki-k3):(Q[8]>>(k364+ki))^(Q[9]<<(k3-ki));\
    R^=(ki>=k2)?Q[9]>>(ki-k2):(Q[8]>>(k264+ki))^(Q[9]<<(k2-ki));\
    R^=(ki>=k1)?Q[9]>>(ki-k1):(Q[8]>>(k164+ki))^(Q[9]<<(k1-ki));\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;\
    P[9]&=mask;

#define REM608_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_640_PENTANOM(Q[9]=Pol[18]>>ki,P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)

#define REM640_PENTANOM(P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask) \
    MACRO_640_PENTANOM(Q[9]=(Pol[18]>>ki)^(Pol[19]<<ki64),P,Pol,k1,k2,k3,ki,ki64,k164,k264,k364,Q,R,mask)


/* Special case k==640 */
#define REM640_PENTANOM_K640(P,Pol,k1,k2,k3,k164,k264,k364,R) \
    ADD640(P,Pol,Pol+10);\
    P[0]^=Pol[10]<<k1;\
    P[1]^=(Pol[10]>>k164)^(Pol[11]<<k1);\
    P[2]^=(Pol[11]>>k164)^(Pol[12]<<k1);\
    P[3]^=(Pol[12]>>k164)^(Pol[13]<<k1);\
    P[4]^=(Pol[13]>>k164)^(Pol[14]<<k1);\
    P[5]^=(Pol[14]>>k164)^(Pol[15]<<k1);\
    P[6]^=(Pol[15]>>k164)^(Pol[16]<<k1);\
    P[7]^=(Pol[16]>>k164)^(Pol[17]<<k1);\
    P[8]^=(Pol[17]>>k164)^(Pol[18]<<k1);\
    P[9]^=(Pol[18]>>k164)^(Pol[19]<<k1);\
    P[0]^=Pol[10]<<k2;\
    P[1]^=(Pol[10]>>k264)^(Pol[11]<<k2);\
    P[2]^=(Pol[11]>>k264)^(Pol[12]<<k2);\
    P[3]^=(Pol[12]>>k264)^(Pol[13]<<k2);\
    P[4]^=(Pol[13]>>k264)^(Pol[14]<<k2);\
    P[5]^=(Pol[14]>>k264)^(Pol[15]<<k2);\
    P[6]^=(Pol[15]>>k264)^(Pol[16]<<k2);\
    P[7]^=(Pol[16]>>k264)^(Pol[17]<<k2);\
    P[8]^=(Pol[17]>>k264)^(Pol[18]<<k2);\
    P[9]^=(Pol[18]>>k264)^(Pol[19]<<k2);\
    P[0]^=Pol[10]<<k3;\
    P[1]^=(Pol[10]>>k364)^(Pol[11]<<k3);\
    P[2]^=(Pol[11]>>k364)^(Pol[12]<<k3);\
    P[3]^=(Pol[12]>>k364)^(Pol[13]<<k3);\
    P[4]^=(Pol[13]>>k364)^(Pol[14]<<k3);\
    P[5]^=(Pol[14]>>k364)^(Pol[15]<<k3);\
    P[6]^=(Pol[15]>>k364)^(Pol[16]<<k3);\
    P[7]^=(Pol[16]>>k364)^(Pol[17]<<k3);\
    P[8]^=(Pol[17]>>k364)^(Pol[18]<<k3);\
    P[9]^=(Pol[18]>>k364)^(Pol[19]<<k3);\
    R=Pol[19]>>k364;\
    R^=Pol[19]>>k264;\
    R^=Pol[19]>>k164;\
    P[0]^=R;\
    P[0]^=R<<k1;\
    P[0]^=R<<k2;\
    P[0]^=R<<k3;

#endif