#include "convNTL.hpp"
#include <NTL/GF2X.h>
#include <NTL/GF2XFactoring.h>
#include <NTL/GF2E.h>

#include "config_gf2n.h"


#define SET_COEF_I(i) \
    GF2XFromBytes(monomX,F_cp,(long)NB_BYTES_GFqn);\
    conv(monom,monomX);\
    SetCoeff(F_NTL,i,monom);


/*  Input:
        F a HFE polynomial

    Output:
        F_NTL the same polynomial, with type GF2EX
        GF2E is initialised to GF(2^n)
*/
void convHFEpolynomToNTLGF2EX(GF2EX& F_NTL,cst_sparse_monic_gf2nx F)
{
    const unsigned char* F_cp=(const unsigned char*)F;
    unsigned int i,j,qi;

    /* Creation of GF2E == GF(2^n) */
    GF2X f=GF2X(HFEn,1);
    SetCoeff(f,0);
    #ifdef __PENTANOMHFE__
        SetCoeff(f,K1);
        SetCoeff(f,K2);
    #endif
    SetCoeff(f,K3);
    GF2E::init(f);
    f.kill();

    GF2X monomX;
    GF2E monom;

    /* Constant */
    SET_COEF_I(0);

    /* X^1 */
    F_cp+=(NB_WORD_GFqn<<3);
    SET_COEF_I(1);

    /* X^(2^i + 2^j), i<HFEDegI, j<=i */
    for(i=0;i<HFEDegI;++i)
    {
        qi=(1U<<i);
        for(j=0;j<=i;++j)
        {
            F_cp+=(NB_WORD_GFqn<<3);
            SET_COEF_I(qi+(1U<<j));
        }
    }

    /* X^(2^HFEDegI + 2^j), j<HFEDegJ */
    #if HFEDegJ
        qi=(1U<<i);
        for(j=0;j<HFEDegJ;++j)
        {
            F_cp+=(NB_WORD_GFqn<<3);
            SET_COEF_I(qi+(1U<<j));
        }
    #endif

    /* Leading term: monic */
    set(monom);
    SetCoeff(F_NTL,HFEDeg,monom);

    monomX.kill();
    clear(monom);
}


#define CONV_VEC_TO_UINT(NAME,NB_WORD_n) \
void NAME(vecn_gf2 res,const vec_GF2& S) \
{\
    const UINT* S_conv=(const UINT*)(S.rep.elts());\
    unsigned int j;\
\
    for(j=0;j<NB_WORD_n;++j)\
    {\
        res[j]=S_conv[j];\
    }\
}

CONV_VEC_TO_UINT(convNTLvecn_GF2ToUINT,NB_WORD_GFqn);


#define CONV_UINT_TO_VEC(NAME,n,NB_WORD_n) \
void NAME(vec_GF2& res,cst_vecn_gf2 S) \
{\
    UINT* S_conv;\
    unsigned int j;\
\
    res.SetLength(n);\
    S_conv = (UINT*)(res.rep.elts());\
\
    for(j=0;j<NB_WORD_n;++j)\
    {\
        S_conv[j]=S[j];\
    }\
}

CONV_UINT_TO_VEC(convUINTToNTLvecn_GF2,HFEn,NB_WORD_GFqn);


#define CONV_UINT_TO_MAT(NAME,nb_row,n,NB_WORD_n) \
void NAME(mat_GF2& res,cst_Mn_gf2 S) \
{\
    UINT* S_conv;\
    unsigned int i,j;\
\
    res.SetDims(nb_row,n);\
\
    /* for each row */\
    for(i=0;i<nb_row;++i)\
    {\
        S_conv = (UINT*)(res[i].rep.elts());\
        /* for each word of the ith row */\
        for(j=0;j<NB_WORD_n;++j)\
        {\
            S_conv[j]=S[i*NB_WORD_n+j];\
        }\
    }\
}

CONV_UINT_TO_MAT(convUINTToNTLmatn_GF2,HFEn,HFEn,NB_WORD_GFqn);
CONV_UINT_TO_MAT(convUINTToNTLmatMZ_GF2,HFEIP,HFEn,NB_WORD_GFqn);
