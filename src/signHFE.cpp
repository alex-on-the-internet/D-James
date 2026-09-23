#include "signHFE.hpp"

BEGIN_EXTERNC
    #include <libXKCP.a.headers/SimpleFIPS202.h>
END_EXTERNC
#include "add.h"
#include "init.h"
#include "randombytes.h"
#include "chooseRootHFE.hpp"
#include "convNTL.hpp"

#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>

using namespace NTL;


/* IP modifier: adds to F the correction associated with the guess z=z_idx of
   the hidden variables. In characteristic 2, calling it a second time with the
   same z_idx removes the correction.

     F[0]             ^= sum_{i<=j, z_i=z_j=1} HQ[i][j]
     F_linear_slot[t] ^= sum_{j: z_j=1} MBilin[t][j],   t=0..HFEDegI

   MBilin has HFEDegI+1 rows of HFEIP elements of GF(2^n), and HQ stores its
   NB_MONOM_IP elements in triangular order. F_linear_slot[t] is the address
   in F of the coefficient of X^(2^t). */
static void apply_ip_z_correction(UINT* F, UINT* const* F_linear_slot,
                                  const UINT* MBilin, const UINT* HQ,
                                  UINT z_idx)
{
    unsigned int t,i,j;
    const UINT* HQ_cp;

    for(t=0;t<=HFEDegI;++t)
    {
        for(j=0;j<HFEIP;++j)
        {
            if((z_idx>>j)&1U)
            {
                HFEADD2(F_linear_slot[t],MBilin+(t*HFEIP+j)*NB_WORD_GFqn);
            }
        }
    }

    HQ_cp=HQ;
    for(i=0;i<HFEIP;++i)
    {
        for(j=i;j<HFEIP;++j)
        {
            if(((z_idx>>i)&1U)&&((z_idx>>j)&1U))
            {
                HFEADD2(F,HQ_cp);
            }
            HQ_cp+=NB_WORD_GFqn;
        }
    }
}


#if HFEDRAGON
/* Dragon terms: adds to F (or removes it, in characteristic 2) the correction
   associated with the hash Y of the message and salt:

     F_linear_slot[t] ^= sum_{j: Y_j=1} DragonL[t][j],   t=0..HFEDegI

   DragonL has HFEDegI+1 rows of HASH_LENGTH elements of GF(2^n). Only the
   linear coefficients of F depend on Y. */
static void apply_dragon_y_correction(UINT* const* F_linear_slot,
                                      const UINT* DragonL,
                                      const UINT* Y)
{
    unsigned int t,j,jq,jr;
    const UINT* row;

    for(t=0;t<=HFEDegI;++t)
    {
        row=DragonL+(size_t)t*HASH_LENGTH*NB_WORD_GFqn;
        for(j=0;j<HASH_LENGTH;++j)
        {
            jq=j/NB_BITS_UINT;
            jr=j%NB_BITS_UINT;
            if((Y[jq]>>jr)&1UL)
            {
                HFEADD2(F_linear_slot[t],row+(size_t)j*NB_WORD_GFqn);
            }
        }
    }
}
#endif


/* Input:
    m the document to sign
    len its size in bytes
    sk the concatenation of the HFE polynomial, the IP data, the Dragon data
    (D-James only), S^(-1) and T^(-1)

  Output:
    sm8 the signature: the n bits of x*S^(-1), followed by the salt byte for
    D-James if DRAGON_EXPLICIT_SALT
    return 0 if the signature is computed, ERROR_ALLOC if an allocation
    fails, SIGNATURE_FAILED if no root is found for any salt (D-James only)

  James: the target is D||R, where D is the hash of the message (m bits) and R
  is random padding (n-m bits, drawn again at each attempt). It is multiplied
  by T^(-1) to get U, and x is a root of F(X)=U.

  D-James: for each salt, Y=H(m||salt) is embedded in the polynomial (see
  apply_dragon_y_correction), and x is a root of F_Y(X)=U, with U the image
  of the fixed public all-ones vector c through T^(-1) (see the note in
  sign_openHFE.c on why the target must be non-zero). 

  With the IP modifier, F is completed by a guess z of the hidden variables,
  and a root x is accepted only if MZ.x==z. The 2^HFEIP guesses are tried in
  turn. */
int signHFE(unsigned char* sm8, const unsigned char* m, size_t len, \
            const UINT* sk)
{
    UINT sm[SIZE_SIGN_UNCOMPRESSED]={0};
    static_vecn_gf2 DR[NB_WORD_GFqn];
    unsigned int k;

    vec_GF2 c_vec,U_vec;
    mat_GF2 S_inv;

    /* Address in F of the coefficient of X^(2^t), t=0..HFEDegI. F is stored as
       [X^0][X^1][X^(2^i+2^j) terms in increasing exponent], so this
       coefficient is at index (t*(t+1))/2+1. */
    UINT* F_linear_slot[HFEDegI+1];
    const UINT* MBilin_ptr=sk+NB_UINT_HFEPOLY+MATRIXip_SIZE;
    const UINT* HQ_ptr=sk+NB_UINT_HFEPOLY+MATRIXip_SIZE+MLip_GFqn_SIZE;
    mat_GF2 MZ_gf2;
    vec_GF2 ip_x_vec,ip_z_vec;
    unsigned int ip_t,ip_bi;
    UINT z_idx;
    int ip_found;

    /* Working copy of the HFE polynomial: sk is shared by all signatures and
       must not change. */
    UINT* F=MALLOC_HFEPOLY;
    VERIFY_ALLOC_RET(F);

    for(k=0;k<NB_UINT_HFEPOLY;++k)
    {
        F[k]=sk[k];
    }

    for(ip_t=0;ip_t<=HFEDegI;++ip_t)
    {
        F_linear_slot[ip_t]=F+NB_WORD_GFqn*(((ip_t*(ip_t+1))>>1)+1);
    }
    /* z = MZ.x */
    convUINTToNTLmatMZ_GF2(MZ_gf2,sk+NB_UINT_HFEPOLY);

    S_inv.SetDims(HFEn,HFEn);
    convUINTToNTLmatn_GF2(S_inv,sk+NB_UINT_HFEPOLY+NB_UINT_IP+NB_UINT_DRAGON);

#if HFEDRAGON

    {
        const UINT* DragonL_ptr=sk+NB_UINT_HFEPOLY+NB_UINT_IP;
        unsigned char* msg_salt;
        UINT Y[SIZE_DIGEST_UINT];
        unsigned int salt,nb_root;
        int found;

        mat_GF2 T_inv;
        T_inv.SetDims(HFEn,HFEn);
        convUINTToNTLmatn_GF2(T_inv,sk+NB_UINT_HFEPOLY+NB_UINT_IP+NB_UINT_DRAGON+MATRIXn_SIZE);
        static_gf2n U[NB_WORD_GFqn];

        vec_GF2 c_vec,U_vec;

        /* c = 1...1 */
        c_vec.SetLength(HFEn);
        for(k=0;k<HFEn;++k)
        {
            c_vec[k]=1;
        }

        #if RIGHT_MULTIPLICATION_BY_T
            mul(U_vec,c_vec,T_inv);
        #else
            mul(U_vec, T_inv, c_vec);
        #endif
        convNTLvecn_GF2ToUINT(U,U_vec);

        msg_salt=(unsigned char*)malloc(len+1);
        VERIFY_ALLOC_RET(msg_salt);
        for(k=0;k<len;++k)
        {
            msg_salt[k]=m[k];
        }

        found=0;
        for(salt=0;(!found)&&(salt<MAX_SALT);++salt)
        {
            msg_salt[len]=(unsigned char)salt;
            /* Y = H(m||salt): the digest has HASH_LENGTH bits */
            HASH((unsigned char*)Y,msg_salt,len+1);

            apply_dragon_y_correction(F_linear_slot,DragonL_ptr,Y);

            /* z=0 */
            nb_root=chooseRootHFE(DR,F,U);
            ip_found=0;
            if(nb_root)
            {
                convUINTToNTLvecn_GF2(ip_x_vec,DR);
                mul(ip_z_vec,MZ_gf2,ip_x_vec);
                ip_found=1;
                for(ip_bi=0;ip_bi<HFEIP;++ip_bi)
                {
                    if(!IsZero(ip_z_vec[ip_bi]))
                    {
                        ip_found=0;
                        break;
                    }
                }
            }
            for(z_idx=1;(!ip_found)&&(z_idx<(1U<<HFEIP));++z_idx)
            {
                apply_ip_z_correction(F,F_linear_slot,MBilin_ptr,HQ_ptr,z_idx);
                nb_root=chooseRootHFE(DR,F,U);
                if(nb_root)
                {
                    convUINTToNTLvecn_GF2(ip_x_vec,DR);
                    mul(ip_z_vec,MZ_gf2,ip_x_vec);
                    ip_found=1;
                    for(ip_bi=0;ip_bi<HFEIP;++ip_bi)
                    {
                        if((unsigned int)IsOne(ip_z_vec[ip_bi])!=((z_idx>>ip_bi)&1U))
                        {
                            ip_found=0;
                            break;
                        }
                    }
                }
                else
                {
                    nb_root=0;
                }
                if(!ip_found)
                {
                    apply_ip_z_correction(F,F_linear_slot,MBilin_ptr,HQ_ptr,z_idx);
                }
            }
            found=ip_found;

            /* Undo the correction of Y before the next salt */
            if(!found)
            {
                apply_dragon_y_correction(F_linear_slot,DragonL_ptr,Y);
            }
            else
            {
                msg_salt[len]=(unsigned char)salt;
            }
        }

        free(msg_salt);

        if(!found)
        {
            S_inv.kill();
            MZ_gf2.kill();
            free(F);
            return SIGNATURE_FAILED;
        }

#if DRAGON_EXPLICIT_SALT
        sm8[NB_BYTES_GFqn]=(unsigned char)(salt-1);
#endif
    }

#else /* James */

    {
        static_gf2n U[NB_WORD_GFqn];
        UINT Hi[SIZE_DIGEST_UINT];
        unsigned int nb_root;
        mat_GF2 T_inv;

        #if(HFEn>HFEm)
            unsigned char* DR_cp=(unsigned char*)DR;
            #if(HFEm&7)
                unsigned char rem_char;
            #endif
        #endif

        T_inv.SetDims(HFEn,HFEn);
        convUINTToNTLmatn_GF2(T_inv,sk+NB_UINT_HFEPOLY+NB_UINT_IP+NB_UINT_DRAGON+MATRIXn_SIZE);

        /* Compute H = H(m) */
        HASH((unsigned char*)Hi,m,len);

        do
        {
            /* D is rebuilt at each attempt: chooseRootHFE overwrites DR
               with the root it finds, even when the attempt fails. */
            HFECOPY_M(DR,Hi);

            #if(HFEm&7)
                /* Clean the last word of D */
                DR[NB_WORD_GFqm-1]&=HFE_MASKm;
                /* Save the last byte of D, which the random draw below
                   overwrites */
                #if(HFEn>HFEm)
                    rem_char=DR_cp[NB_BYTES_GFqm-1];
                #endif
            #endif

            #if(HFEn>HFEm)
                #if(HFEm&7)
                    /* Random padding R, after the last byte of D */
                    randombytes(DR_cp+NB_BYTES_GFqm-1,
                            (unsigned long long)(NB_BYTES_GFqn-NB_BYTES_GFqm+1));
                    /* Erase the low bits of that byte, then restore D */
                    DR_cp[NB_BYTES_GFqm-1]&=~((1U<<(HFEm&7))-1);
                    DR_cp[NB_BYTES_GFqm-1]^=rem_char;
                #else
                    /* Random padding R */
                    randombytes(DR_cp+NB_BYTES_GFqm,
                            (unsigned long long)(NB_BYTES_GFqn-NB_BYTES_GFqm));
                #endif

                /* Clean the last word */
                #if (HFEn&7)
                    DR[NB_WORD_GFqn-1]&=HFE_MASKn;
                #endif
            #endif

            /* U = T^(-1) * (D||R) */
            convUINTToNTLvecn_GF2(c_vec,DR);
            #if RIGHT_MULTIPLICATION_BY_T
                mul(U_vec,c_vec,T_inv);
            #else
                mul(U_vec, T_inv, c_vec);
            #endif
            convNTLvecn_GF2ToUINT(U,U_vec);

            /* Find x with F(x)=U for some guess z of the IP variables:
               z=0 first */
            nb_root=chooseRootHFE(DR,F,U);
            ip_found=0;
            if(nb_root)
            {
                convUINTToNTLvecn_GF2(ip_x_vec,DR);
                mul(ip_z_vec,MZ_gf2,ip_x_vec);
                ip_found=1;
                for(ip_bi=0;ip_bi<HFEIP;++ip_bi)
                {
                    if(!IsZero(ip_z_vec[ip_bi]))
                    {
                        ip_found=0;
                        break;
                    }
                }
            }
            for(z_idx=1;(!ip_found)&&(z_idx<(1U<<HFEIP));++z_idx)
            {
                apply_ip_z_correction(F,F_linear_slot,MBilin_ptr,HQ_ptr,z_idx);
                nb_root=chooseRootHFE(DR,F,U);
                if(nb_root)
                {
                    convUINTToNTLvecn_GF2(ip_x_vec,DR);
                    mul(ip_z_vec,MZ_gf2,ip_x_vec);
                    ip_found=1;
                    for(ip_bi=0;ip_bi<HFEIP;++ip_bi)
                    {
                        if((unsigned int)IsOne(ip_z_vec[ip_bi])!=((z_idx>>ip_bi)&1U))
                        {
                            ip_found=0;
                            break;
                        }
                    }
                }
                if(!ip_found)
                {
                    /* Undo the correction of this z */
                    apply_ip_z_correction(F,F_linear_slot,MBilin_ptr,HQ_ptr,z_idx);
                    nb_root=0;
                }
            }

            if(!nb_root)
            {
                /* Failure: retry with another R */
                continue;
            }

            break;
        } while(1);

        T_inv.kill();
    }

#endif /* HFEDRAGON */

    convUINTToNTLvecn_GF2(U_vec,DR);

    /* S||X = x * S^(-1) */
    mul(c_vec,U_vec,S_inv);
    convNTLvecn_GF2ToUINT(sm,c_vec);

    S_inv.kill();
    U_vec.kill();
    c_vec.kill();

    MZ_gf2.kill();
    ip_x_vec.kill();
    ip_z_vec.kill();
    free(F);

    /* Take the n first bits */
    for(k=0;k<NB_BYTES_GFqn;++k)
    {
        sm8[k]=((unsigned char*)sm)[k];
    }

    return 0;
}
