#ifndef _MUL_H
#define _MUL_H

/* Multiplication and squaring in GF(2)[X] for the sizes used by hfemul and
   hfesquare (see config_HFE.h). Products are computed by the gf2x library and
   squares by table lookup. */

#include <stdint.h>
#include "arch.h"

#include <gf2x/gf2x_mul1.h>
#include <gf2x/gf2x_mul2.h>
#include <gf2x/gf2x_mul3.h>
#include <gf2x/gf2x_mul4.h>
#include <gf2x/gf2x_mul5.h>
#include <gf2x/gf2x_mul6.h>
#include <gf2x/gf2x_mul7.h>
#include <gf2x/gf2x_mul8.h>
#include <gf2x/gf2x_mul9.h>

#define mul128(C,A,B) gf2x_mul2(C,A,B)
#define mul192(C,A,B) gf2x_mul3(C,A,B)
#define mul256(C,A,B) gf2x_mul4(C,A,B)
#define mul320(C,A,B) gf2x_mul5(C,A,B)
#define mul384(C,A,B) gf2x_mul6(C,A,B)
#define mul448(C,A,B) gf2x_mul7(C,A,B)
#define mul512(C,A,B) gf2x_mul8(C,A,B)
#define mul576(C,A,B) gf2x_mul9(C,A,B)
#define mul640(C,A,B) gf2x_mul((unsigned long*)(C),(const unsigned long*)(A),10,(const unsigned long*)(B),10)

/* Sizes that are not a multiple of 64 bits use the product of the next size */
#define mul96 mul128
#define mul160 mul192
#define mul224 mul256
#define mul288 mul320
#define mul352 mul384
#define mul416 mul448
#define mul480 mul512
#define mul544 mul576
#define mul608 mul640

#define square96 SQUARE96
#define square128 SQUARE128
#define square160 SQUARE160
#define square192 SQUARE192
#define square224 SQUARE224
#define square256 SQUARE256
#define square288 SQUARE288
#define square320 SQUARE320
#define square352 SQUARE352
#define square384 SQUARE384
#define square416 SQUARE416
#define square448 SQUARE448
#define square480 SQUARE480
#define square512 SQUARE512
#define square544 SQUARE544
#define square576 SQUARE576
#define square608 SQUARE608
#define square640 SQUARE640


/* Squaring in GF(2)[X] inserts a zero between consecutive bits */

/* tab_square[i]: square of the polynomial i (8 bits) in GF(2)[X] */
static const uint64_t tab_square[256]={0,1,4,5,16,17,20,21,64,65,68,69,80,81,84,85,256,257,260,261,272,273,276,277,320,321,324,325,336,337,340,341,1024,1025,1028,1029,1040,1041,1044,1045,1088,1089,1092,1093,1104,1105,1108,1109,1280,1281,1284,1285,1296,1297,1300,1301,1344,1345,1348,1349,1360,1361,1364,1365,4096,4097,4100,4101,4112,4113,4116,4117,4160,4161,4164,4165,4176,4177,4180,4181,4352,4353,4356,4357,4368,4369,4372,4373,4416,4417,4420,4421,4432,4433,4436,4437,5120,5121,5124,5125,5136,5137,5140,5141,5184,5185,5188,5189,5200,5201,5204,5205,5376,5377,5380,5381,5392,5393,5396,5397,5440,5441,5444,5445,5456,5457,5460,5461,16384,16385,16388,16389,16400,16401,16404,16405,16448,16449,16452,16453,16464,16465,16468,16469,16640,16641,16644,16645,16656,16657,16660,16661,16704,16705,16708,16709,16720,16721,16724,16725,17408,17409,17412,17413,17424,17425,17428,17429,17472,17473,17476,17477,17488,17489,17492,17493,17664,17665,17668,17669,17680,17681,17684,17685,17728,17729,17732,17733,17744,17745,17748,17749,20480,20481,20484,20485,20496,20497,20500,20501,20544,20545,20548,20549,20560,20561,20564,20565,20736,20737,20740,20741,20752,20753,20756,20757,20800,20801,20804,20805,20816,20817,20820,20821,21504,21505,21508,21509,21520,21521,21524,21525,21568,21569,21572,21573,21584,21585,21588,21589,21760,21761,21764,21765,21776,21777,21780,21781,21824,21825,21828,21829,21840,21841,21844,21845};


#define SQUARE32(C,A) \
    (C)=tab_square[(A)&255];\
    (C)^=tab_square[((A)>>8)&255]<<16;\
    (C)^=tab_square[((A)>>16)&255]<<32;\
    (C)^=tab_square[(A)>>24]<<48;

#define SQUARE64(C,A) \
    (C)[0]=tab_square[(A)&255];\
    (C)[0]^=tab_square[((A)>>8)&255]<<16;\
    (C)[0]^=tab_square[((A)>>16)&255]<<32;\
    (C)[0]^=tab_square[((A)>>24)&255]<<48;\
\
    (C)[1]=tab_square[((A)>>32)&255];\
    (C)[1]^=tab_square[((A)>>40)&255]<<16;\
    (C)[1]^=tab_square[((A)>>48)&255]<<32;\
    (C)[1]^=tab_square[(A)>>56]<<48;

#define SQUARE96(C,A) \
    SQUARE64(C,(A)[0]);\
    SQUARE32((C)[2],(A)[1]);

#define SQUARE128(C,A) \
    SQUARE64(C,(A)[0]);\
    SQUARE64(C+2,(A)[1]);

#define SQUARE160(C,A) \
    SQUARE128(C,A);\
    SQUARE32((C)[4],(A)[2]);

#define SQUARE192(C,A) \
    SQUARE128(C,A);\
    SQUARE64(C+4,(A)[2]);

#define SQUARE224(C,A) \
    SQUARE128(C,A);\
    SQUARE96(C+4,A+2);

#define SQUARE256(C,A) \
    SQUARE128(C,A);\
    SQUARE128(C+4,A+2);

#define SQUARE288(C,A) \
    SQUARE256(C,A);\
    SQUARE32((C)[8],(A)[4]);

#define SQUARE320(C,A) \
    SQUARE256(C,A);\
    SQUARE64(C+8,(A)[4]);

#define SQUARE352(C,A) \
    SQUARE256(C,A);\
    SQUARE96(C+8,A+4);

#define SQUARE384(C,A) \
    SQUARE256(C,A);\
    SQUARE128(C+8,A+4);

#define SQUARE416(C,A) \
    SQUARE256(C,A);\
    SQUARE160(C+8,A+4);

#define SQUARE448(C,A) \
    SQUARE256(C,A);\
    SQUARE192(C+8,A+4);

#define SQUARE480(C,A) \
    SQUARE256(C,A);\
    SQUARE224(C+8,A+4);

#define SQUARE512(C,A) \
    SQUARE256(C,A);\
    SQUARE256(C+8,A+4);

#define SQUARE544(C,A) \
    SQUARE384(C,A);\
    SQUARE160(C+12,A+6);

#define SQUARE576(C,A) \
    SQUARE384(C,A);\
    SQUARE192(C+12,A+6);

#define SQUARE608(C,A) \
    SQUARE384(C,A);\
    SQUARE224(C+12,A+6);

#define SQUARE640(C,A) \
    SQUARE384(C,A);\
    SQUARE256(C+12,A+6);

#endif
