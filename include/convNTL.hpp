#ifndef _CONVNTL_HPP
#define _CONVNTL_HPP

#include "config_HFE.h"
#include <NTL/GF2EX.h>
#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>


using namespace NTL;

/* Also initialises GF2E to GF(2^n) */
void convHFEpolynomToNTLGF2EX(GF2EX& F_NTL,cst_sparse_monic_gf2nx F);

void convNTLvecn_GF2ToUINT(vecn_gf2 res,const vec_GF2& S);
void convUINTToNTLvecn_GF2(vec_GF2& res,cst_vecn_gf2 S);

/* Matrix n*n (S and T) */
void convUINTToNTLmatn_GF2(mat_GF2& res,cst_Mn_gf2 S);
/* Matrix MZ, HFEIP*n */
void convUINTToNTLmatMZ_GF2(mat_GF2& res,cst_Mn_gf2 S);

#endif
