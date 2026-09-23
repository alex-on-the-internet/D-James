#ifndef _TYPES_HFE_H
#define _TYPES_HFE_H

/* Types used to store field elements, vectors, matrices and quadratic forms.
   All of them are arrays of UINT (64-bit words). */

/* Element of GF(2^n) */
typedef UINT* gf2n;
typedef const UINT* cst_gf2n;
/* To declare data[length] */
typedef UINT static_gf2n;

/* Vector of elements of GF(2^n) */
typedef UINT* vec_gf2n;

/* Univariate polynomial of GF(2^n)[X] with coefficients stored in increasing
   degree order. Only the coefficients that can be non-zero are stored, and
   the leading coefficient, equal to 1, is not. */
typedef UINT* sparse_monic_gf2nx;
typedef const UINT* cst_sparse_monic_gf2nx;

/* Vector of n bits (signature, secret variables) */
typedef UINT* vecn_gf2;
typedef const UINT* cst_vecn_gf2;
typedef UINT static_vecn_gf2;

/* Vector of m bits (hash) */
typedef UINT* vecm_gf2;
typedef UINT static_vecm_gf2;

/* Vector of HFEN=n+ny bits: the input (x||Y) of the public quadratic form of
   D-James. HFEN==HFEn for James. */
typedef UINT* vecN_gf2;
typedef const UINT* cst_vecN_gf2;
typedef UINT static_vecN_gf2;

/* Matrix n*n in GF(2) (S and T) */
typedef UINT* Mn_gf2;
typedef const UINT* cst_Mn_gf2;

/* Invertible matrix n*n in GF(2) */
typedef UINT* GLn_gf2;
typedef const UINT* cst_GLn_gf2;

/* Lower triangular matrix n*n in GF(2) */
typedef UINT* Tn_gf2;

/* Quadratic form in the variables (x||Y) with coefficients in GF(2^n), i.e. n
   quadratic equations over GF(2) (Y is empty for James). The constant comes
   first, then, for each x_i in turn (rows i=0..n-1), the coefficients of
   x_i x_j for j=i..HFEN-1. There is no row for the Y variables, since a Y_i
   does not multiply another Y_j. */
typedef UINT* mqsn_gf2n;
typedef const UINT* cst_mqsn_gf2n;

/* Public quadratic system with m equations, computed monomial by monomial
   during key generation: for each monomial (same order as mqsn_gf2n), a
   vector of m bits, bit e being its coefficient in equation e. */
typedef UINT* mqsn_gf2m;
typedef const UINT* cst_mqsn_gf2m;

/* Public key: for each of the m equations, a vector of NB_MONOM_PK bits, bit
   t being the coefficient of the monomial number t (same order as mqsn_gf2n),
   on NB_WORD_MONOM_PK words. */
typedef UINT* pk_gf2;
typedef const UINT* cst_pk_gf2;

#endif
