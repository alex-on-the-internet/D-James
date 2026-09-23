#include "sign_keypairHFE.hpp"
#include "init.h"
#include "config_HFE.h"

#include "convNTL.hpp"
#include "genHFEtab.hpp"
#include "genSecretMQS.hpp"

#include "config_gf2n.h"

#include "changeEquationMQS.h"
#include "changeVariableMQS.h"
#if HFEDRAGON
    #include "changeVariableMQS_dragon.h"
#endif
#include "invMatrix.h"

#include <string.h>
#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>
#include <NTL/GF2EX.h>
#include <NTL/GF2XFactoring.h>


/*
 * Generates a keypair.
 *
 * Output:
 *   0           if the generation succeeds
 *   ERROR_ALLOC if a memory allocation fails
 *
 *   pk : public quadratic system, with m equations in HFEN variables
 *        (HFEN==HFEn for James, n+ny for D-James: the x and Y variables)
 *
 *   sk : concatenation of:
 *        - the HFE polynomial
 *        - the IP data (MZ, MBilin, HQ)
 *        - the Dragon data DragonL (D-James only)
 *        - S^(-1)
 *        - T^(-1)
 *
 *        S^(-1) and T^(-1) are stored as matrices over GF(2).
 */


/*
 * Format of the public key:
 *
 * The public key is stored equation by equation. Each equation is a vector of
 * NB_MONOM_PK bits (padded to a whole number of words): bit t is the
 * coefficient of the monomial number t. The monomials are numbered in this
 * order: the constant, then x_i x_j row by row (i, then j>=i). For D-James,
 * the variables are x_0..x_{n-1} then Y_0..Y_{ny-1}, and there is no row for
 * the Y variables, since a Y_i does not multiply another Y_j.
 *
 * Example with n = 4, m = 2 and no Y variable (11 monomials):
 *
 *   monomial number   0    1     2     3     4     5     6     7     8     9     10
 *   monomial          1    x0    x0x1  x0x2  x0x3  x1    x1x2  x1x3  x2    x2x3  x3
 *   equation 0        1    1     0     1     0     0     0     1     1     0     1
 *   equation 1        1    0     1     1     1     1     0     0     1     1     0
 *
 * i.e. the two equations are:
 *
 *   1 + x0 + x0x2 + x1x3 + x2 + x3
 *
 *   1 + x0x1 + x0x2 + x0x3 + x1 + x2 + x2x3
 */


/*
 * Format of the secret key:
 *
 * The HFE polynomial is stored as the concatenation of its non-zero
 * coefficients X^0, X^1, X^(2^i + 2^j), in increasing order, with j <= i.
 * The IP data and the Dragon data follow.
 *
 * S^(-1) and T^(-1) are stored row by row. A row may use several UINT words;
 * for example, a row of 128 GF(2) elements uses two words.
 *
 * Example of T with n = 4:
 *
 *       1 0 0 1
 *       1 1 1 1
 *       0 0 1 0
 *       0 1 0 1
 *
 *   T[0] = 0x9
 *   T[1] = 0xF
 *   T[2] = 0x4
 *   T[3] = 0xA
 */


int sign_keypairHFE(UINT* pk, UINT* sk)
{
    /* Initialise GF2E to GF(2^n): chooseRootHFE, used when signing, expects
       it to be initialised. */
    GF2X f(HFEn, 1);

    SetCoeff(f, 0);

#ifdef __PENTANOMHFE__
    SetCoeff(f, K1);
    SetCoeff(f, K2);
#endif

    SetCoeff(f, K3);

    GF2E::init(f);
    f.kill();


    mqsn_gf2n Q;
    UINT *S,*S_tmp;
    int ret;

    /* HFE polynomial, IP data and Dragon data */
    genHFEpolynom(sk);
    genHFEpolynomIP(sk + NB_UINT_HFEPOLY);
#if HFEDRAGON
    genHFEpolynomDragon(sk + NB_UINT_HFEPOLY + NB_UINT_IP);
#endif

    /* Secret quadratic form, with the IP and Dragon terms */
    Q = MALLOC_MQSn;
    if (!Q)
    {
        return ERROR_ALLOC;
    }

    ret = genSecretMQS(Q, sk);
    if (ret)
    {
        free(Q);
        return ret;
    }

    S = MALLOC_MATRIXn;
    S_tmp = MALLOC_MATRIXn;
    if (!S || !S_tmp)
    {
        free(Q);
        free(S);
        free(S_tmp);
        return ERROR_ALLOC;
    }

    /* Linear transformation S on the variables. invMatrix modifies its
       second argument, so S_inv is computed from a copy. */
    ret = genRandomInvertibleMatrixnLU(S);
    if (ret)
    {
        free(Q);
        free(S);
        free(S_tmp);
        return ret;
    }

    memcpy(S_tmp, S, MATRIXn_SIZE * sizeof(UINT));
    invMatrix(sk + NB_UINT_HFEPOLY + NB_UINT_IP + NB_UINT_DRAGON, S_tmp);

#if HFEDRAGON
    changeVariableMQS_dragon(Q, S);
#else
    changeVariableMQS(Q, S);
#endif

    /* Linear transformation T on the equations */
    ret = genRandomInvertibleMatrixnLU(S);
    if (ret)
    {
        free(Q);
        free(S);
        free(S_tmp);
        return ret;
    }

    memcpy(S_tmp, S, MATRIXn_SIZE * sizeof(UINT));
    invMatrix(sk + NB_UINT_HFEPOLY + NB_UINT_IP + NB_UINT_DRAGON + MATRIXn_SIZE,
              S_tmp);

    /* Public key: the first m equations of T*Q */
    ret = changeEquationMQS(pk, Q, S);

    free(Q);
    free(S);
    free(S_tmp);

    return ret;
}
