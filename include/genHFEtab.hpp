#ifndef _GENHFETAB_HPP
#define _GENHFETAB_HPP

#include "config_HFE.h"

/* Random HFE polynomial: NB_UINT_HFEPOLY words */
void genHFEpolynom(sparse_monic_gf2nx F);

/* Random secret data of the IP modifier (MZ || MBilin || HQ):
   NB_UINT_IP words */
void genHFEpolynomIP(UINT* sk_ip);

#if HFEDRAGON
/* Random secret data DragonL of the Dragon terms: NB_UINT_DRAGON words */
void genHFEpolynomDragon(UINT* sk_dragon);
#endif

/* Random invertible matrix n*n in GF(2), built as L*U */
int genRandomInvertibleMatrixnLU(GLn_gf2 S);

#endif
