#include "genHFEtab.hpp"
#include <stdlib.h>
#include "randombytes.h"
#include "init.h"

/* Random generation of the secret key components */


/* Input: F of size NB_UINT_HFEPOLY words */
/* Output: random monic HFE polynomial, coefficients in GF(2^n) */
void genHFEpolynom(sparse_monic_gf2nx F)
{
    /* Random coefficients; the leading term is 1 and is not stored */
    randombytes((unsigned char*)F,NB_UINT_HFEPOLY<<3);

    /* Clean the last word of each element of GF(2^n) */
    #if HFEnr
        sparse_monic_gf2nx F_cp=F-1;
        unsigned int i;
        for(i=0;i<NB_COEFS_HFEPOLY;++i)
        {
            F_cp+=NB_WORD_GFqn;
            *F_cp&=HFE_MASKn;
        }
    #endif
}


/* Input: sk_ip, a buffer of NB_UINT_IP words
   Output: sk_ip = MZ || MBilin || HQ:
    - MZ: HFEIP*HFEn bits in GF(2), z = MZ.x (uniformly random, it does not
      need to be invertible)
    - MBilin: (HFEDegI+1)*HFEIP elements of GF(2^n), coefficients of X^(2^t)*z_j
    - HQ: NB_MONOM_IP elements of GF(2^n), triangular quadratic form in z */
void genHFEpolynomIP(UINT* sk_ip)
{
    UINT *MZ_cp=sk_ip;
    unsigned char* MZ_conv;
    unsigned int i;

    for(i=0;i<HFEIP;++i)
    {
        MZ_conv=(unsigned char*)MZ_cp;
        randombytes(MZ_conv,NB_BYTES_GFqn);
        #if HFEnr8
            MZ_conv[NB_BYTES_GFqn-1]&=HFE_MASKn8;
        #endif
        MZ_cp+=NB_WORD_GFqn;
    }

    /* MBilin and HQ: random elements of GF(2^n), like the HFE coefficients */
    randombytes((unsigned char*)MZ_cp,(MLip_GFqn_SIZE+MQip_GFqn_SIZE)<<3);
    #if HFEnr
    {
        UINT *cp=MZ_cp-NB_WORD_GFqn;
        unsigned int nb=(HFEDegI+1)*HFEIP+NB_MONOM_IP;
        for(i=0;i<nb;++i)
        {
            cp+=NB_WORD_GFqn;
            cp[NB_WORD_GFqn-1]&=HFE_MASKn;
        }
    }
    #endif
}


#if HFEDRAGON
/* Input: sk_dragon, a buffer of NB_UINT_DRAGON words
   Output: sk_dragon = DragonL, (HFEDegI+1)*HASH_LENGTH random elements of
   GF(2^n), coefficients of X^(2^t)*Y_j */
void genHFEpolynomDragon(UINT* sk_dragon)
{
    unsigned int i;

    randombytes((unsigned char*)sk_dragon,NB_UINT_DRAGON<<3);
    #if HFEnr
    {
        UINT *cp=sk_dragon-NB_WORD_GFqn;
        unsigned int nb=(HFEDegI+1)*HASH_LENGTH;
        for(i=0;i<nb;++i)
        {
            cp+=NB_WORD_GFqn;
            cp[NB_WORD_GFqn-1]&=HFE_MASKn;
        }
    }
    #endif
}
#endif


/* Output: random lower triangular matrix with 1 on the diagonal */
#define GENLOWMATRIX(NAME,LTRIANGULAR_SIZE,nq,nr) \
static void NAME(Tn_gf2 L) \
{\
    Tn_gf2 L_cp=L;\
    UINT mask;\
    unsigned int iq,ir;\
\
    /* >>1 <<3 == <<2 */\
    randombytes((unsigned char*)L,LTRIANGULAR_SIZE<<3);\
\
    /* for each row */\
    for(iq=1;iq<=nq;++iq)\
    {\
        mask=0;\
        for(ir=0;ir<NB_BITS_UINT;++ir)\
        {\
            /* Put the bit of diagonal to 1 + zeros before the diagonal */\
            *L_cp&=mask;\
            *L_cp^=1UL<<ir;\
            mask<<=1;\
            ++mask;\
\
            L_cp+=iq;\
        }\
        /* Next column */\
        ++L_cp;\
    }\
\
    /* iq = HFEnq */\
    mask=0;\
    for(ir=0;ir<nr;++ir)\
    {\
        /* Put the bit of diagonal to 1 + zeros before the diagonal */\
        *L_cp&=mask;\
        *L_cp^=1UL<<ir;\
        mask<<=1;\
        ++mask;\
\
        L_cp+=iq;\
    }\
}

GENLOWMATRIX(genLowerMatrixn,LTRIANGULAR_N_SIZE,HFEnq,HFEnr);


#define LOOPJR(NB_IT) \
    mini=(iq<jq)?iq:jq;\
    *S_cp=0UL;\
    for(jr=0;jr<NB_IT;++jr)\
    {\
        /* Dot product */\
        tmp=L_cp[0]&U_cp[0];\
        for(k=1;k<mini;++k)\
        {\
            tmp^=L_cp[k]&U_cp[k];\
        }\
        XORBITS64(tmp);\
        *S_cp^=tmp<<jr;\
        U_cp+=jq;\
    }\
    ++S_cp;


#define LOOPIR(NB_IT,nq,nr,REM) \
    for(ir=0;ir<NB_IT;++ir)\
    {\
        U_cp=U;\
        /* for each row of U (multiply by the transpose) */\
        for(jq=1;jq<=nq;++jq)\
        {\
            LOOPJR(NB_BITS_UINT);\
        }\
        REM;\
        L_cp+=iq;\
    }


/* Input: matrix of size (HFEn*NB_WORD_GFqn) UINT */
/* Output: invertible random matrix n*n in GF(2), S = L*U
           ERROR_ALLOC if a malloc fails, else 0 */
#define GENRANDINVMATRIXLU(NAME,LTRIANGULAR_SIZE,genLowerMatrix,nq,nr,REM) \
int NAME(GLn_gf2 S) \
{\
    Tn_gf2 L,L_cp,U,U_cp;\
    GLn_gf2 S_cp=S;\
    UINT tmp;\
    unsigned int iq,ir,jq,jr,k,mini;\
\
    /* Generation of L and U */\
    L=(UINT*)malloc(LTRIANGULAR_SIZE*sizeof(UINT));\
    VERIFY_ALLOC_RET(L);\
    U=(UINT*)malloc(LTRIANGULAR_SIZE*sizeof(UINT));\
    if(!U)\
    {\
        free(L);\
        return ERROR_ALLOC;\
    }\
\
    genLowerMatrix(L);\
    /* Generate the transpose of U */\
    genLowerMatrix(U);\
\
    /* Computation of S = L*U */\
    L_cp=L;\
    /* for each row of L (and S) */\
    for(iq=1;iq<=nq;++iq)\
    {\
        LOOPIR(NB_BITS_UINT,nq,nr,REM);\
    }\
    LOOPIR(nr,nq,nr,REM);\
\
    free(L);\
    free(U);\
    return 0;\
}

#if HFEnr
GENRANDINVMATRIXLU(genRandomInvertibleMatrixnLU,LTRIANGULAR_N_SIZE,genLowerMatrixn,HFEnq,HFEnr,LOOPJR(HFEnr));
#else
GENRANDINVMATRIXLU(genRandomInvertibleMatrixnLU,LTRIANGULAR_N_SIZE,genLowerMatrixn,HFEnq,HFEnr,);
#endif
