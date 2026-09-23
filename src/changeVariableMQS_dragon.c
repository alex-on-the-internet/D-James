#include "changeVariableMQS_dragon.h"
#if HFEDRAGON
#include "changeVariableMQS.h"
#include <stdlib.h>
#include "add.h"
#include "init.h"
#include "bit.h"

/*
 * D-James: the public quadratic form has HFEN=HFEn+HASH_LENGTH variables
 * (x||Y), and the change of variables S only acts on x. The secret form Q has
 * non-zero coefficients only in the (x,x) block (HFE and IP terms) and in the
 * (x,Y) block (Dragon terms), so
 *
 *     S_full * Q * S_full^T = [ Sx.Qxx.Sx^T   Sx.Qxy ]
 *                             [     0            0   ]
 *
 * The constant and the (x,x) block are masked by changeVariableMQS, and the
 * (x,Y) block is multiplied on the left by Sx. The Y rows stay zero.
 *
 * Qc is the compact upper triangular form (constant, then Q[i][i..HFEN-1] for
 * each row i), modified in place. Only used in key generation, so the loops
 * are not optimised.
 */
int changeVariableMQS_dragon(mqsn_gf2n Qc, cst_GLn_gf2 Sx)
{
    mqsn_gf2n Qxx,Qxx_cp;
    UINT *Qxy_dense,*Qxy_out,*out_row,*Qxy_cp;
    const UINT *Qc_cp,*Sx_row;
    unsigned int i,j,a,iq,ir;

    /* Step 1: extract the constant and the (x,x) block into Qxx */
    Qxx=(UINT*)malloc((size_t)NB_MONOM*NB_WORD_GFqn*sizeof(UINT));
    VERIFY_ALLOC_RET(Qxx);

    Qc_cp=Qc;
    Qxx_cp=Qxx;

    /* constant */
    HFECOPY(Qxx_cp,Qc_cp);
    Qc_cp+=NB_WORD_GFqn;
    Qxx_cp+=NB_WORD_GFqn;

    /* Keep columns i..HFEn-1 of each row, skip the Y columns after them */
    for(i=0;i<HFEn;++i)
    {
        for(j=i;j<HFEn;++j)
        {
            HFECOPY(Qxx_cp,Qc_cp);
            Qc_cp+=NB_WORD_GFqn;
            Qxx_cp+=NB_WORD_GFqn;
        }
        /* skip the Y columns of this row */
        Qc_cp+=(size_t)HASH_LENGTH*NB_WORD_GFqn;
    }

    /* Step 2: mask Qxx as in James */
    changeVariableMQS(Qxx,Sx);

    /* Step 3: extract the (x,Y) block Qxy into a dense buffer */
    Qxy_dense=(UINT*)malloc((size_t)HFEn*HASH_LENGTH*NB_WORD_GFqn*sizeof(UINT));
    if(!Qxy_dense)
    {
        free(Qxx);
        return ERROR_ALLOC;
    }

    Qc_cp=Qc+NB_WORD_GFqn; /* skip constant */
    Qxy_cp=Qxy_dense;
    for(i=0;i<HFEn;++i)
    {
        /* skip the x columns of this row */
        Qc_cp+=(size_t)(HFEn-i)*NB_WORD_GFqn;
        /* Y columns of row i */
        for(j=0;j<HASH_LENGTH;++j)
        {
            HFECOPY(Qxy_cp,Qc_cp);
            Qc_cp+=NB_WORD_GFqn;
            Qxy_cp+=NB_WORD_GFqn;
        }
    }

    /* Step 4: Qxy' = Sx * Qxy: row a of Qxy' is the sum of the rows i of Qxy
       such that bit i of row a of Sx is set */
    Qxy_out=(UINT*)calloc((size_t)HFEn*HASH_LENGTH*NB_WORD_GFqn,sizeof(UINT));
    if(!Qxy_out)
    {
        free(Qxx);
        free(Qxy_dense);
        return ERROR_ALLOC;
    }

    for(a=0;a<HFEn;++a)
    {
        out_row=Qxy_out+(size_t)a*HASH_LENGTH*NB_WORD_GFqn;
        Sx_row=Sx+(size_t)a*NB_WORD_GFqn;

        for(i=0;i<HFEn;++i)
        {
            iq=i/NB_BITS_UINT;
            ir=i%NB_BITS_UINT;
            if((Sx_row[iq]>>ir)&1UL)
            {
                const UINT *in_row=Qxy_dense+(size_t)i*HASH_LENGTH*NB_WORD_GFqn;
                unsigned int w;
                for(w=0;w<HASH_LENGTH*NB_WORD_GFqn;++w)
                {
                    out_row[w]^=in_row[w];
                }
            }
        }
    }
    free(Qxy_dense);

    /* Step 5: write the result back into Qc */
    Qxx_cp=Qxx;

    {
        UINT *Qc_wcp=Qc;

        /* constant */
        HFECOPY(Qc_wcp,Qxx_cp);
        Qc_wcp+=NB_WORD_GFqn;
        Qxx_cp+=NB_WORD_GFqn;

        for(i=0;i<HFEn;++i)
        {
            for(j=i;j<HFEn;++j)
            {
                HFECOPY(Qc_wcp,Qxx_cp);
                Qc_wcp+=NB_WORD_GFqn;
                Qxx_cp+=NB_WORD_GFqn;
            }
            Qxy_cp=Qxy_out+(size_t)i*HASH_LENGTH*NB_WORD_GFqn;
            for(j=0;j<HASH_LENGTH;++j)
            {
                HFECOPY(Qc_wcp,Qxy_cp);
                Qc_wcp+=NB_WORD_GFqn;
                Qxy_cp+=NB_WORD_GFqn;
            }
        }
    }

    free(Qxx);
    free(Qxy_out);
    return 0;
}
#endif
