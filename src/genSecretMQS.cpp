#include "genSecretMQS.hpp"
#include "genCanonicalBasis.h"
#include "arith.h"
#include "add.h"
#include "init.h"
#include <stdlib.h>

/*
 * Builds the secret quadratic form in the compact layout (mqsn_gf2n): the
 * constant, then for the rows i=0..HFEn-1 the HFEN-i coefficients
 * Q[i][i..HFEN-1], which changeVariableMQS(_dragon) and changeEquationMQS
 * consume. The rows of the Y variables are zero and are not stored.
 *
 * theta_i is the i-th element of the polynomial basis (theta_0=1,
 * theta_i=alpha^i). X^(2^t) is linear in the theta_i^(2^t), so the HFE
 * polynomial is a quadratic form in the coordinates of X. genCanonicalBasis()
 * computes each theta_i^(2^t) once, and every contribution below is then a
 * single hfemulmod().
 */


/* alpha_vec has (HFEDegI+1) rows of (HFEn-1) elements: row t holds
   theta_1^(2^t), ..., theta_(HFEn-1)^(2^t) (see genCanonicalBasis.c).
   theta_0^(2^t)=1 is not stored. */
static const UINT ONE_ELEMENT[NB_WORD_GFqn] = {1};

static const UINT* theta_pow(const UINT* alpha_vec, unsigned int t, unsigned int idx)
{
    if(idx == 0)
    {
        return ONE_ELEMENT;
    }
    return alpha_vec + ((size_t)t * (HFEn - 1) + (idx - 1)) * NB_WORD_GFqn;
}


/*
 * IP modifier: replaces z by MZ.x in the terms HQ(z)=z^T.HQ.z and
 * B(X,z)=sum_t X^(2^t).(MBilin[t,:].z), and adds them to the HFEn*HFEn block
 * of Qd. The first loop handles HQ(z), which becomes a quadratic form in x;
 * the second one handles B(X,z), which adds terms to the X^(2^t)
 * coordinates.
 */
static void fold_ip_terms(
    UINT* Qd, unsigned int N, const UINT* alpha_vec, const UINT* F2)
{
    const UINT* MZ = F2 + NB_UINT_HFEPOLY;
    const UINT* MBilin = F2 + NB_UINT_HFEPOLY + MATRIXip_SIZE;
    const UINT* MQuad = F2 + NB_UINT_HFEPOLY + MATRIXip_SIZE + MLip_GFqn_SIZE;
    UINT* BilinX;
    unsigned int a, b, zi, zj, t, k;

    /* Part quadratic in z:
         Qd[a][b] ^= sum_{i<=j, MZ[i][a]=1, MZ[j][b]=1} MQuad[i][j] */
    for(a = 0; a < HFEn; ++a)
    {
        unsigned int a_word = a / NB_BITS_UINT, a_bit = a % NB_BITS_UINT;

        for(b = 0; b < HFEn; ++b)
        {
            unsigned int b_word = b / NB_BITS_UINT, b_bit = b % NB_BITS_UINT;
            const UINT* MQuad_cp = MQuad;

            for(zi = 0; zi < HFEIP; ++zi)
            {
                int bit_a = (MZ[zi * NB_WORD_GFqn + a_word] >> a_bit) & 1U;

                for(zj = zi; zj < HFEIP; ++zj)
                {
                    if(bit_a &&
                       ((MZ[zj * NB_WORD_GFqn + b_word] >> b_bit) & 1U))
                    {
                        HFEADD2(Qd + ((size_t)a * N + b) * NB_WORD_GFqn, MQuad_cp);
                    }
                    MQuad_cp += NB_WORD_GFqn;
                }
            }
        }
    }

    /* Part bilinear in z: BilinX[t][k] = sum_j MZ[j][k] * MBilin[t][j],
       then Qd[i2][k] ^= theta_i2^(2^t) * BilinX[t][k] */
    BilinX = (UINT*)calloc((size_t)(HFEDegI + 1) * HFEn * NB_WORD_GFqn, sizeof(UINT));
    if(!BilinX)
    {
        return;
    }

    for(t = 0; t <= HFEDegI; ++t)
    {
        const UINT* MBilin_row = MBilin + (size_t)t * HFEIP * NB_WORD_GFqn;

        for(k = 0; k < HFEn; ++k)
        {
            unsigned int k_word = k / NB_BITS_UINT, k_bit = k % NB_BITS_UINT;
            UINT* dst = BilinX + ((size_t)t * HFEn + k) * NB_WORD_GFqn;

            for(zj = 0; zj < HFEIP; ++zj)
            {
                if((MZ[zj * NB_WORD_GFqn + k_word] >> k_bit) & 1U)
                {
                    HFEADD2(dst, MBilin_row + (size_t)zj * NB_WORD_GFqn);
                }
            }
        }
    }

    for(t = 0; t <= HFEDegI; ++t)
    {
        for(a = 0; a < HFEn; ++a)
        {
            const UINT* th = theta_pow(alpha_vec, t, a);
            UINT contrib[NB_WORD_GFqn];

            for(k = 0; k < HFEn; ++k)
            {
                hfemulmod(contrib, th, BilinX + ((size_t)t * HFEn + k) * NB_WORD_GFqn);
                HFEADD2(Qd + ((size_t)a * N + k) * NB_WORD_GFqn, contrib);
            }
        }
    }

    free(BilinX);
}


#if HFEDRAGON
/*
 * Dragon terms: adds the bilinear x-Y terms to the top-right HFEn*HASH_LENGTH
 * block of Qd (columns HFEn..N-1). There is no Y*Y term, so the rows and
 * columns of the Y variables stay zero.
 */
static void fold_dragon_terms(
    UINT* Qd, unsigned int N, const UINT* alpha_vec, const UINT* F2)
{
    const UINT* DragonL = F2 + NB_UINT_HFEPOLY + NB_UINT_IP;
    unsigned int a, b, t;

    for(t = 0; t <= HFEDegI; ++t)
    {
        const UINT* row = DragonL + (size_t)t * HASH_LENGTH * NB_WORD_GFqn;

        for(a = 0; a < HFEn; ++a)
        {
            const UINT* th = theta_pow(alpha_vec, t, a);
            UINT contrib[NB_WORD_GFqn];

            for(b = 0; b < HASH_LENGTH; ++b)
            {
                hfemulmod(contrib, th, row + (size_t)b * NB_WORD_GFqn);
                HFEADD2(Qd + ((size_t)a * N + (HFEn + b)) * NB_WORD_GFqn, contrib);
            }
        }
    }
}
#endif


/* Input:
    F2 : the secret key (HFE polynomial, IP data, then Dragon data for D-James)

  Output:
    Q : the secret quadratic form (NB_MONOM_PK coefficients), allocated by the
        caller
    return ERROR_ALLOC if an allocation fails, 0 otherwise.
*/
int genSecretMQS(mqsn_gf2n Q, const UINT* F2)
{
    UINT* alpha_vec;
    UINT* Qd;
    const UINT* F2_cp;
    UINT tmp1[NB_WORD_GFqn];
    unsigned int i2, j2;
    unsigned int i, j;

    alpha_vec = (UINT*)calloc(
        (size_t)(HFEDegI + 1) * (HFEn - 1) * NB_WORD_GFqn, sizeof(UINT)
    );
    if(!alpha_vec)
    {
        return ERROR_ALLOC;
    }
    genCanonicalBasis(alpha_vec);

    /* Dense HFEN*HFEN working buffer. Qd[i2][j2] and Qd[j2][i2] are
       accumulated separately and folded into the triangular Q at the end. */
    Qd = (UINT*)calloc((size_t)HFEN * HFEN * NB_WORD_GFqn, sizeof(UINT));
    if(!Qd)
    {
        free(alpha_vec);
        return ERROR_ALLOC;
    }

    /* The constant is the coefficient of X^0: the IP and Dragon terms do not
       change it, since z and Y always appear multiplied by some x_i. */
    F2_cp = F2 + NB_WORD_GFqn;

    /* X^1: Qd[i2][i2] ^= theta_i2 * coeff(X^1) */
    for(i2 = 0; i2 < HFEn; ++i2)
    {
        hfemulmod(tmp1, theta_pow(alpha_vec, 0, i2), F2_cp);
        HFEADD2(Qd + ((size_t)i2 * HFEN + i2) * NB_WORD_GFqn, tmp1);
    }
    F2_cp += NB_WORD_GFqn;

    for(i = 0; i < HFEDegI; ++i)
    {
        /* Quadratic terms X^(2^i+2^j), j=0..i-1 */
        for(j = 0; j < i; ++j)
        {
            for(i2 = 0; i2 < HFEn; ++i2)
            {
                UINT partial[NB_WORD_GFqn];
                UINT contrib[NB_WORD_GFqn];

                hfemulmod(partial, theta_pow(alpha_vec, i, i2), F2_cp);

                for(j2 = 0; j2 < HFEn; ++j2)
                {
                    hfemulmod(contrib, partial, theta_pow(alpha_vec, j, j2));
                    HFEADD2(Qd + ((size_t)i2 * HFEN + j2) * NB_WORD_GFqn, contrib);
                }
            }
            F2_cp += NB_WORD_GFqn;
        }

        /* Linear term X^(2^(i+1)): the case j==i is linear, since
           x*x=x in GF(2) */
        for(i2 = 0; i2 < HFEn; ++i2)
        {
            hfemulmod(tmp1, theta_pow(alpha_vec, i + 1, i2), F2_cp);
            HFEADD2(Qd + ((size_t)i2 * HFEN + i2) * NB_WORD_GFqn, tmp1);
        }
        F2_cp += NB_WORD_GFqn;
    }

    /* Terms X^(2^HFEDegI+2^j), j=0..HFEDegJ-1 */
    for(j = 0; j < HFEDegJ; ++j)
    {
        for(i2 = 0; i2 < HFEn; ++i2)
        {
            UINT partial[NB_WORD_GFqn];
            UINT contrib[NB_WORD_GFqn];

            hfemulmod(partial, theta_pow(alpha_vec, HFEDegI, i2), F2_cp);

            for(j2 = 0; j2 < HFEn; ++j2)
            {
                hfemulmod(contrib, partial, theta_pow(alpha_vec, j, j2));
                HFEADD2(Qd + ((size_t)i2 * HFEN + j2) * NB_WORD_GFqn, contrib);
            }
        }
        F2_cp += NB_WORD_GFqn;
    }

    /* Leading term X^(2^HFEDegI+2^HFEDegJ): F is monic, its coefficient 1 is
       not stored in F2. */
    for(i2 = 0; i2 < HFEn; ++i2)
    {
        for(j2 = 0; j2 < HFEn; ++j2)
        {
            UINT contrib[NB_WORD_GFqn];

            hfemulmod(
                contrib,
                theta_pow(alpha_vec, HFEDegI, i2),
                theta_pow(alpha_vec, HFEDegJ, j2)
            );
            HFEADD2(Qd + ((size_t)i2 * HFEN + j2) * NB_WORD_GFqn, contrib);
        }
    }

    fold_ip_terms(Qd, HFEN, alpha_vec, F2);

#if HFEDRAGON
    fold_dragon_terms(Qd, HFEN, alpha_vec, F2);
#endif

    free(alpha_vec);

    /* Fold Qd into the compact layout, for the rows a<n only: Qd[a][b] and
       Qd[b][a] both give the coefficient of x_a*x_b (a!=b) and are added; the
       diagonal (x_a*x_a=x_a) is copied. */
    {
        UINT* Q_cp = Q;
        unsigned int a, b;

        HFECOPY(Q_cp, F2);
        Q_cp += NB_WORD_GFqn;

        for(a = 0; a < HFEn; ++a)
        {
            HFECOPY(Q_cp, Qd + ((size_t)a * HFEN + a) * NB_WORD_GFqn);
            Q_cp += NB_WORD_GFqn;

            for(b = a + 1; b < HFEN; ++b)
            {
                UINT folded[NB_WORD_GFqn];

                HFECOPY(folded, Qd + ((size_t)a * HFEN + b) * NB_WORD_GFqn);
                HFEADD2(folded, Qd + ((size_t)b * HFEN + a) * NB_WORD_GFqn);
                HFECOPY(Q_cp, folded);
                Q_cp += NB_WORD_GFqn;
            }
        }
    }

    free(Qd);
    return 0;
}
