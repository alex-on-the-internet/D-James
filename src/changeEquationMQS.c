#include "changeEquationMQS.h"
#include <stdlib.h>
#include <string.h>
#include "init.h"

/*
 * Mixes the equations of a quadratic system: pk = T * MQS.
 *
 * MQS holds, for each monomial, its n coefficients as a vector of GF(2)^n.
 * For each output equation j: pk[j] = sum_i T[j][i] * MQS[i]. T is stored row
 * by row, each row on NB_WORD_GFqn words; only its first m rows are used.
 * The product is first computed monomial by monomial (vectors of GF(2)^m),
 * then transposed to store each equation as a vector of monomials.
 *
 * This assumes T is applied on the left (pk[j]=sum_i T[j][i]*MQS[i], T's
 * rows). With RIGHT_MULTIPLICATION_BY_T (the convention signHFE.cpp and
 * sign_openHFE.c otherwise use throughout: U=c*T^(-1), a row vector times a
 * matrix), the matching relation is instead pk[j]=sum_i MQS[i][j]... no:
 * pk[i][j]=sum_k MQS[i][k]*T[k][j], i.e. T's *columns*. transpose_matrix_n()
 * below builds that transpose once (T is only ever HFEn*HFEn here, and this
 * runs once per keygen), so the rest of this file can stay unchanged and
 * keep reading T row by row.
 */

#define LOOPJR(NB_IT) \
    HFEDOTPRODUCT(tmp,T_cp,MQS_cp); \
    T_cp+=NB_WORD_GFqn; \
    *pk_cp=tmp; \
    for(jr=1;jr<NB_IT;++jr) \
    { \
        HFEDOTPRODUCT(tmp,T_cp,MQS_cp); \
        T_cp+=NB_WORD_GFqn; \
        *pk_cp^=tmp<<jr; \
    } \
    ++pk_cp;

/* In place transpose of a 64*64 bit matrix: bit c of a[r] goes to bit r of
   a[c]. */
static void transpose64(UINT a[64])
{
    static const UINT mask[6]={0x00000000FFFFFFFFUL,0x0000FFFF0000FFFFUL,
        0x00FF00FF00FF00FFUL,0x0F0F0F0F0F0F0F0FUL,0x3333333333333333UL,
        0x5555555555555555UL};
    unsigned int s,j,k;
    UINT t;

    for(s=0,j=32;s<6;++s,j>>=1)
    {
        for(k=0;k<64;k=((k|j)+1)&~j)
        {
            t=((a[k]>>j)^a[k|j])&mask[s];
            a[k|j]^=t;
            a[k]^=t<<j;
        }
    }
}


#if RIGHT_MULTIPLICATION_BY_T
/* out[j][i] = in[i][j], both HFEn*HFEn, stored row by row (NB_WORD_GFqn
   words/row). Built from the same 64*64 block transpose as above; not a hot
   path (keygen only, O(HFEn^2) bit moves). */
static void transpose_matrix_n(UINT* out, cst_Mn_gf2 in)
{
    unsigned int bi,bj,r,c,i,j;
    UINT blk[64];

    memset(out,0,(size_t)HFEn*NB_WORD_GFqn*sizeof(UINT));

    for(bi=0;bi*64<HFEn;++bi)
    {
        for(bj=0;bj*64<HFEn;++bj)
        {
            for(r=0;r<64;++r)
            {
                i=bi*64+r;
                blk[r]=(i<HFEn)?in[i*NB_WORD_GFqn+bj]:0UL;
            }
            transpose64(blk);
            for(c=0;c<64;++c)
            {
                j=bj*64+c;
                if(j<HFEn)
                {
                    out[j*NB_WORD_GFqn+bi]=blk[c];
                }
            }
        }
    }
}
#endif


int changeEquationMQS(
    pk_gf2 pk,
    cst_mqsn_gf2n MQS,
    cst_Mn_gf2 T)
{
    mqsn_gf2m tmp_pk,pk_cp;
    UINT tmp;
    cst_mqsn_gf2n MQS_cp;
    cst_Mn_gf2 T_cp;

    unsigned int i;
    unsigned int jq;
    unsigned int jr;

#if RIGHT_MULTIPLICATION_BY_T
    UINT* T_transposed=(UINT*)malloc((size_t)HFEn*NB_WORD_GFqn*sizeof(UINT));
    VERIFY_ALLOC_RET(T_transposed);
    transpose_matrix_n(T_transposed,T);
    T=T_transposed;
#endif

    tmp_pk = (UINT*)malloc(MQ_MONOM_SIZE*sizeof(UINT));
    VERIFY_ALLOC_RET(tmp_pk);

    MQS_cp = MQS;
    pk_cp = tmp_pk;

    /* For each monomial */
    for(i=0;i<NB_MONOM_PK;++i)
    {
        T_cp = T;

        /* Blocks of NB_BITS_UINT equations */
        for(jq=0;jq<HFEmq;++jq)
        {
            LOOPJR(NB_BITS_UINT);
        }

        /* Remaining equations */
        #if (HFEmr)
            LOOPJR(HFEmr);
        #endif

        MQS_cp += NB_WORD_GFqn;
    }

    /* Transpose: bit e of the monomial t becomes bit t of the equation e.
       The monomials are processed by blocks of 64, and the equations by
       blocks of 64 too. */
    {
        UINT blk[64];
        unsigned int b,k,r,c;

        for(b=0;b<NB_WORD_MONOM_PK;++b)
        {
            for(k=0;k<NB_WORD_GFqm;++k)
            {
                for(r=0;r<64;++r)
                {
                    i=64*b+r;
                    blk[r]=(i<NB_MONOM_PK)?tmp_pk[i*NB_WORD_GFqm+k]:0UL;
                }
                transpose64(blk);
                for(c=0;c<64;++c)
                {
                    i=64*k+c;
                    if(i<HFEm)
                    {
                        pk[i*NB_WORD_MONOM_PK+b]=blk[c];
                    }
                }
            }
        }
    }

    free(tmp_pk);
#if RIGHT_MULTIPLICATION_BY_T
    free(T_transposed);
#endif
    return 0;
}
