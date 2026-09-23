#include "evaluateMQS.hpp"
#include "init.h"
#include "bit.h"

/* V |= the len bits of src from bit number `from`, put at bit number `pos` of
   V. src must have a zero word after its last word, V one word after its
   last word. */
static void append_bits(UINT* V, unsigned int pos, const UINT* src,
                        unsigned int from, unsigned int len)
{
    unsigned int k,take,p,w=from>>6,s=from&63;
    UINT chunk;

    for(k=0;k<len;k+=64)
    {
        chunk=s?((src[w]>>s)|(src[w+1]<<(64-s))):src[w];
        take=len-k;
        if(take<64)
        {
            chunk&=(1UL<<take)-1UL;
        }
        p=pos+k;
        V[p>>6]|=chunk<<(p&63);
        if(p&63)
        {
            V[(p>>6)+1]|=chunk>>(64-(p&63));
        }
        ++w;
    }
}

/*
 * Evaluates the m equations of the public key on the HFEN bits of m.
 *
 * The values of all the monomials are first computed as a vector V of
 * NB_MONOM_PK bits, in the order of the public key. The monomials x_i*x_j of
 * the row i, j=i..HFEN-1, are the bits i..HFEN-1 of the input if x_i=1, and
 * zeros otherwise. Then the equation e is the parity of the bitwise AND
 * between V and the vector of the equation e.
 */
void evaluateMQS_pk(
    vecm_gf2 c,
    cst_vecN_gf2 m,
    cst_pk_gf2 pk)
{
    UINT V[NB_WORD_MONOM_PK+1]={0};
    UINT X[NB_WORD_GFqN+1];
    UINT acc;
    cst_pk_gf2 row;
    unsigned int i,e,w,pos;

    for(w=0;w<NB_WORD_GFqN;++w)
    {
        X[w]=m[w];
    }
    X[NB_WORD_GFqN]=0UL;

    /* constant */
    V[0]=1UL;
    pos=1;
    for(i=0;i<HFEn;++i)
    {
        if((X[i>>6]>>(i&63))&1UL)
        {
            append_bits(V,pos,X,i,HFEN-i);
        }
        pos+=HFEN-i;
    }

    for(w=0;w<NB_WORD_GFqm;++w)
    {
        c[w]=0UL;
    }
    row=pk;
    for(e=0;e<HFEm;++e)
    {
        acc=0UL;
        for(w=0;w<NB_WORD_MONOM_PK;++w)
        {
            acc^=row[w]&V[w];
        }
        XORBITS64(acc);
        c[e>>6]|=acc<<(e&63);
        row+=NB_WORD_MONOM_PK;
    }
}
