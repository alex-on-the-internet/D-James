#include "chooseRootHFE.hpp"
#include "init.h"
#include "bit.h"
#include "convNTL.hpp"
#include <NTL/GF2EXFactoring.h>

BEGIN_EXTERNC
    #include <libXKCP.a.headers/SimpleFIPS202.h>
END_EXTERNC


/* To sort roots */
static void quickSort(vec_gf2n tab,int d,int f)
{
    /* If the length is null */
    if(d>=f)
    {
        return;
    }

    int left=d;
    int right=f;

    while(1)
    {
        while(HFECMP_LT(tab+left,tab+d))
        {
            left+=NB_WORD_GFqn;
        }
        while(HFECMP_GT(tab+right,tab+d))
        {
            right-=NB_WORD_GFqn;
        }

        if(left<right)
        {
            HFEPERMUTATION(tab+left,tab+right);
        } else
        {
            break;
        }
    }

    quickSort(tab,d,right);
    quickSort(tab,right+NB_WORD_GFqn,f);
}


#include "randombytes.h"

/* This function is extracted from NTL 10.5.0 */
static
void TraceMap(GF2EX& h, const GF2EX& a, const GF2EXModulus& F)
{
   GF2EX res, tmp;

   res = a;
   tmp = a;

   long i;
   for (i = 0; i < GF2E::degree()-1; i++) {
      SqrMod(tmp, tmp, F);
      add(res, res, tmp);
   }

   h = res;
}

/* This function is extracted from NTL 10.5.0. The call to rand is replaced
   by a call to randombytes. */
static
void RecFindRoots_with_randombytes(vec_GF2E& x, const GF2EX& f)
{
    unsigned char rand_UINT[NB_BYTES_GFqn];
    GF2X monomX;

   if (deg(f) == 0) return;

   if (deg(f) == 1) {
      long k = x.length();
      x.SetLength(k+1);
      x[k] = ConstTerm(f);
      return;
   }

   GF2EX h;

   GF2E r;

   {
      GF2EXModulus F;
      build(F, f);

      do {
         /* NTL: random(r); */
         randombytes(rand_UINT,NB_BYTES_GFqn);
         #if HFEnr8
             /* Clean the last byte (included the zero padding) */
             rand_UINT[NB_BYTES_GFqn-1]&=HFE_MASKn8;
         #endif
         GF2XFromBytes(monomX,rand_UINT,(long)NB_BYTES_GFqn);
         conv(r,monomX);

         clear(h);
         SetCoeff(h, 1, r);
         TraceMap(h, h, F);
         GCD(h, h, f);
      } while (deg(h) <= 0 || deg(h) == deg(f));
   }

   RecFindRoots_with_randombytes(x, h);
   div(h, f, h);
   RecFindRoots_with_randombytes(x, h);
   monomX.kill();
}

/* This function is extracted from NTL 10.5.0 */
static void FindRoots_with_randombytes(vec_GF2E& x, const GF2EX& ff)
{
   GF2EX f = ff;

   if (!IsOne(LeadCoeff(f)))
      LogicError("FindRoots: bad args");

   x.SetMaxLength(deg(f));
   x.SetLength(0);
   RecFindRoots_with_randombytes(x, f);
}


/*  Input:
        F a HFE polynomial in GF(2^n)[X]
        U an element of GF(2^n)

    Output:
        the number of roots of F-U
        root a root of F-U. If there are several, the choice is deterministic:
        the roots are sorted and the one of index H(U) mod (number of roots)
        is taken.

    Requirement: F is monic
*/
unsigned int chooseRootHFE(gf2n root, cst_sparse_monic_gf2nx F, cst_gf2n U)
{
    GF2EXModulus F_NTL_opt;
    GF2EX F_NTL,Xqn,Pgcd;
    vec_GF2E roots_NTL;
    GF2X monomX;
    GF2E monom;

    unsigned int l;
    unsigned int i;

    UINT hash[SIZE_DIGEST_UINT];


    /*  F_HFE(x) */
    convHFEpolynomToNTLGF2EX(F_NTL,F);
    /* The previous function initialises the modulus of GF2E */

    /* U */
    GF2XFromBytes(monomX,(unsigned char*)U,(long)NB_BYTES_GFqn);
    conv(monom,monomX);

    /*  F_HFE(x) - U = 0 */
    add(F_NTL,F_NTL,monom);

    /* Optimize the modular reduction by F_NTL */
    build(F_NTL_opt,F_NTL);
    F_NTL.kill();
    /* X^(2^n) - X mod F */
    FrobeniusMap(Xqn,F_NTL_opt);
    add(Xqn,Xqn,GF2EX(1,1));
    GCD(Pgcd,F_NTL_opt,Xqn);

    Xqn.kill();
    l=(unsigned int)deg(Pgcd);
    if(!l)
    {
        monomX.kill();
        clear(monom);
        Pgcd.kill();
        return 0;
    }

    FindRoots_with_randombytes(roots_NTL,Pgcd);
    Pgcd.kill();

    if(l==1)
    {
        /* Unique root */
        conv(monomX,roots_NTL[0]);
        BytesFromGF2X((unsigned char*)root,monomX,(long)NB_BYTES_GFqn);
        /* To clean the last word */
        #if HFEnr
            root[NB_WORD_GFqn-1]&=HFE_MASKn;
        #endif
    } else
    {
        /* Conversion of roots in UINT* */
        static_gf2n roots_UINT[NB_WORD_GFqn*l];
        for(i=0;i<l;++i)
        {
            BytesFromGF2X((unsigned char*)(roots_UINT+i*NB_WORD_GFqn),conv<GF2X>(roots_NTL[i]),NB_WORD_GFqn<<3);
            /* Clean the last word */
            #if HFEnr
                roots_UINT[(i+1)*NB_WORD_GFqn-1]&=HFE_MASKn;
            #endif
        }

        /* Sort the roots */
        quickSort(roots_UINT,0,(l-1)*NB_WORD_GFqn);

        /* Choose a root with a deterministic hash */
        HASH((unsigned char*)hash,(unsigned char*)U,NB_BYTES_GFqn);
        HFECOPY(root,roots_UINT+(hash[0]%l)*NB_WORD_GFqn);
    }

    monomX.kill();
    clear(monom);
    return l;
}
