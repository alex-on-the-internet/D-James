#include "sign_openHFE.h"
#include "config_HFE.h"
BEGIN_EXTERNC
    #include <libXKCP.a.headers/SimpleFIPS202.h>
END_EXTERNC
#include "add.h"
#include "init.h"
#include "evaluateMQS.hpp"


#if HFEDRAGON
/* out = x||Y: the HFEn bits of sig followed by the HASH_LENGTH bits of Y
   (HFEN bits in total), the input of the public quadratic form. */
static void pack_sig_and_Y(vecN_gf2 out, cst_vecn_gf2 sig, const UINT* Y)
{
    unsigned int i,j;

    for(i=0;i<NB_WORD_GFqN;++i)
    {
        out[i]=0UL;
    }

    for(i=0;i<HFEn;++i)
    {
        if((sig[i/NB_BITS_UINT]>>(i%NB_BITS_UINT))&1UL)
        {
            out[i/NB_BITS_UINT]|=(1UL<<(i%NB_BITS_UINT));
        }
    }
    for(j=0;j<HASH_LENGTH;++j)
    {
        if((Y[j/NB_BITS_UINT]>>(j%NB_BITS_UINT))&1UL)
        {
            i=HFEn+j;
            out[i/NB_BITS_UINT]|=(1UL<<(i%NB_BITS_UINT));
        }
    }
}
#endif


/* Input:
    m the document
    len its size in bytes
    sm8 the signature
    pk the public key: a quadratic form in HFEN variables with m equations

  Output:
    0 for a valid signature, !=0 otherwise

  James: the signature is valid if pk(sig) is equal to H(m).

  D-James: the signature is valid if pk(sig||Y)=0, with Y=H(m||salt). The salt
  is read from the signature if DRAGON_EXPLICIT_SALT, otherwise all the
  MAX_SALT values are tried.
*/
int sign_openHFE(const unsigned char* m, size_t len, const unsigned char* sm8,
                 const UINT* pk)
{
    UINT sm[SIZE_SIGN_UNCOMPRESSED]={0};
    unsigned int i;

    /* The n first bits; the salt byte, if any, follows */
    for(i=0;i<NB_BYTES_GFqn;++i)
    {
        ((unsigned char*)sm)[i]=sm8[i];
    }


#if HFEDRAGON
    {
        static_vecm_gf2 Si[NB_WORD_GFqm];
        static_vecN_gf2 sig_and_Y[NB_WORD_GFqN];
        static_gf2n c[NB_WORD_GFqm];
        memset(c,0xFF,sizeof(c));
        #if (HFEmr)
            c[NB_WORD_GFqm-1] &= (1UL << (HFEm % NB_BITS_UINT))-1;
        #endif
        UINT Y[SIZE_DIGEST_UINT];
        unsigned char* msg_salt;
        unsigned int salt,salt_start,salt_end;
        int valid;

        msg_salt=(unsigned char*)malloc(len+1);
        VERIFY_ALLOC_RET(msg_salt);
        for(i=0;i<len;++i)
        {
            msg_salt[i]=m[i];
        }

#if DRAGON_EXPLICIT_SALT
        salt_start=(unsigned int)sm8[NB_BYTES_GFqn];
        salt_end=salt_start+1U;
#else
        salt_start=0U;
        salt_end=MAX_SALT;
#endif

        valid=0;
        for(salt=salt_start;(!valid)&&(salt<salt_end);++salt)
        {
            msg_salt[len]=(unsigned char)salt;
            HASH((unsigned char*)Y,msg_salt,len+1);

            pack_sig_and_Y(sig_and_Y,sm,Y);
            evaluateMQS_pk(Si,sig_and_Y,pk);

            valid=HFEISEQUALm(Si,c);
        }

        free(msg_salt);

        return !valid;
    }
#else
    {
        static_vecm_gf2 Si[NB_WORD_GFqm];
        UINT D[SIZE_DIGEST_UINT];

        /* The m first bits of H(m) */
        HASH((unsigned char*)D,m,len);
        #if HFEmr
            D[NB_WORD_GFqm-1]&=HFE_MASKm;
        #endif

        evaluateMQS_pk(Si,sm,pk);

        return !HFEISEQUALm(Si,D);
    }
#endif
}
