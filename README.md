# D-James and James: ultra-short multivariate signatures

Reference implementation of **James** and **D-James**, the signature schemes of the paper
*D-James: Ultra Short Multivariate Signatures* (`DJames_paper.pdf`).

Both schemes are HFE-based and repair GeMSS, which was broken by the MinRank-S attack. They use the
HFE<sup>-</sup>IP construction, which combines the *minus* modifier (some public equations are
hidden) with the *internal perturbation* (IP) modifier. D-James adds **Dragon terms**: bilinear
terms between the secret variables and the hash variables. They decouple the number of public
equations from the hash length, so the signature can be about half as long as the hash output.

| Scheme  | Security | Signature (paper)                    | Sign / verify (paper)      |
|---------|----------|--------------------------------------|----------------------------|
| D-James | 128 bits | 189 bits, or 156 bits when truncated | 0.81 ms / 63 µs           |
| D-James | 256 bits | 390 bits, or 348 bits when truncated | 4.36 ms / 1.36 ms           |
| James   | 128 bits | 283 bits, or 238 bits when truncated | 1.91 ms / 59 µs           |
| James   | 256 bits | 578 bits, or 523 bits when truncated | 8.39 ms / 781 µs            |

Timings are those of Table 9 of the paper (median of 100 runs, Intel Core Ultra 5 235U). This is
research code: it is **not constant-time** and must not be used to protect real data.


## The schemes in short

Notation: `q = 2`; `n` is the degree of the extension field GF(2^n); `a` is the number of minus
equations, so the public key has `m = n - a` equations; `r` is the number of IP modifiers; `D` is
the degree of the central polynomial; `ny` is the number of hash variables (D-James only).

**Central map.** As in HFE, the secret key contains a sparse univariate polynomial over GF(2^n)
whose monomials are `X^(2^i + 2^j)` with `2^i + 2^j <= D`. Seen over GF(2), it is a quadratic map.
It is hidden by two secret invertible linear maps `S` and `T`. Both schemes use `D = 5`, i.e. the
monomials `X`, `X^3` and `X^5`.

**Minus and IP.** Only the first `m = n - a` equations are published. The IP modifier adds `r`
random linear forms `z = MZ.x` of the secret variables, with bilinear terms in `(X, z)` and
quadratic terms in `z`. Signing guesses `z` (2^r guesses), inverts the resulting HFE polynomial and
keeps a root that is consistent with the guess. All parameter sets use `r = 2`.

**James.** The hash `D = H(M)` has `m` bits. It is padded with `n - m` random bits `R`, and the
signer finds `x` with `F(x) = T^-1 (D || R)`. The signature is `x S^-1`. Verification checks
`P(sig) = H(M)`. The random padding replaces the salt of HFE.

**D-James.** The hash `Y = H(M || salt)` has `ny = 2K` bits and appears in the central polynomial
through Dragon terms `sum_t L_t(Y) X^(2^t)`. After fixing `Y`, the polynomial is an ordinary HFE
polynomial, and signing looks for a **non-zero root** of `F_Y(X) = 0`; if there is none, another
salt is tried. The public system is quadratic in `(x, Y)` and has `m` equations. The signature is
`x S^-1`, and verification checks `P(sig, Y) = 0`. Since `ny` no longer has to be close to `m`, a
128-bit-secure scheme needs only `n = 189` signature bits instead of about 256.

See Sections 2, 3 and 7 of the paper for details, and Section 6 for the choice of parameters and
the security analysis.


## Parameter sets

One source tree, four parameter sets (all with `q = 2`, `r = 2`, `D = 5`), selected at compile time:

| Make target    | Scheme  | Security | n   | m   | a  | ny  | Public key (paper) |
|----------------|---------|----------|-----|-----|----|-----|--------------------|
| `d-james-128`  | D-James | 128 bits | 189 | 162 | 27 | 256 | 1.35 MB            |
| `d-james-256`  | D-James | 256 bits | 390 | 324 | 66 | 512 | 11.1 MB            |
| `james-128`    | James   | 128 bits | 283 | 256 | 27 | –   | 1.29 MB            |
| `james-256`    | James   | 256 bits | 578 | 512 | 66 | –   | 10.7 MB            |

The paper also gives parameter sets for `q = 4, 5, 13, 23`. They are not in this repository, because
the field arithmetic here is specialised to GF(2^n). The independent Python/Rust implementation
mentioned in the paper (<https://github.com/mjosaarinen/xdjames>) covers `q > 2`.

**Sizes in bytes** (as produced by this code):

| Make target   | Public key | Secret key | Signature |
|---------------|-----------:|-----------:|----------:|
| `d-james-128` | 1 343 952  | 27 888     | 24        |
| `d-james-256` | 11 176 704 | 130 592    | 49        |
| `james-128`   | 1 286 144  | 23 280     | 36        |
| `james-256`   | 10 711 040 | 93 760     | 73        |

The public key contains only the non-zero coefficients: `1 + n(n+1)/2 + n·ny` monomials (the
Dragon terms are bilinear, so there is no `Y·Y` coefficient), each with `m` bits. It is stored
equation by equation, and each equation is padded to a whole number of 64-bit words (at most 63
bits), which explains the small difference with the exact bit counts (for example 1 343 385 bytes
for `d-james-128`).

**Difference with the paper.** The paper reports `|sig_fast| = n` bits and a truncated `|sig_short|`,
whose missing bits are recovered by a hybrid Gröbner-basis computation at verification. Only the
first form is implemented. The secret key also stores `T^-1`, which D-James signing does not use.


## Building

### Dependencies

| Library | Debian/Ubuntu package | Used for                                      |
|---------|-----------------------|-----------------------------------------------|
| NTL     | `libntl-dev`          | factorisation of the central polynomial       |
| GMP     | `libgmp-dev`          | required by NTL                               |
| gf2x    | `libgf2x-dev`         | multiplication in GF(2)[x]                    |
| OpenSSL | `libssl-dev`          | AES, for the NIST DRBG in `src/rng.c`         |
| XKCP    | –                     | SHA-3 (Keccak); git submodule, built by `make` |

XKCP is included as a git submodule (`third_party/XKCP`), and it has a submodule itself. Building it
also needs `xsltproc`. Tested with g++ 13, NTL 11.5.1, gf2x 1.3.0 and OpenSSL 3.0 on Ubuntu 24.04.

### Compilation and benchmark

```sh
git clone --recurse-submodules <repository-url>
cd DJames
make                    # the four parameter sets; or: make d-james-128
./build/d-james-128/bench
```

`make` first builds XKCP (`generic64` target), then one `bench` binary per parameter set in
`build/<set>/`. Useful variables: `XKCP_TARGET=AVX2` selects another XKCP target, `XKCP_DIR=...`
uses an existing XKCP build instead of the submodule, and `LOCAL=/opt/local` gives the prefix of
NTL, GMP and gf2x. `make clean` removes `build/`.

`bench` generates a key pair, signs and verifies a message 100 times, checks each result and prints
the median time of each operation:

```
========== D-James benchmark ==========
HFEn             : 189
Tests            : 100
Median KeyGen    : ... ms
Median Sign      : ... ms
Median Verify    : ... ms
Signature verification: SUCCESS
```

The random generator is seeded with a fixed value, so runs are reproducible.


## Using the library

`include/api.h` exposes the NIST signature API:

```c
int crypto_sign_keypair(unsigned char *pk, unsigned char *sk);
int crypto_sign(unsigned char *sm, unsigned long long *smlen,
                const unsigned char *m, unsigned long long mlen,
                const unsigned char *sk);
int crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                     const unsigned char *sm, unsigned long long smlen,
                     const unsigned char *pk);
```

`CRYPTO_PUBLICKEYBYTES`, `CRYPTO_SECRETKEYBYTES` and `CRYPTO_BYTES` give the sizes of the buffers.
The signed message `sm` is the signature (`CRYPTO_BYTES` bytes) followed by the message.
`crypto_sign_open` returns 0 if the signature is valid. Call `randombytes_init` (`src/rng.c`) before
generating keys or signatures; `src/bench.c` shows a complete example.


## Changing the parameters

The parameter sets are defined at the top of `include/config_HFE.h`, one block per set:

| Macro                          | Meaning                                                      |
|--------------------------------|--------------------------------------------------------------|
| `K`                            | security level; selects the hash function (SHA3-256/384/512) |
| `HFEn`                         | field degree `n`, with `65 <= n <= 640`                      |
| `HFEDELTA`                     | number of minus equations `a`                                |
| `HFEDRAGON`                    | 1 for D-James, 0 for James                                   |
| `HFEIP`                        | number of IP modifiers `r` (at least 1)                      |
| `HFEDeg`, `HFEDegI`, `HFEDegJ` | `D = 2^HFEDegI + 2^HFEDegJ`, with `HFEDegI > HFEDegJ`        |
| `DRAGON_EXPLICIT_SALT`         | D-James only, default 0 (see below)                          |

For D-James, the number of hash variables is `ny = 2K`. To add a set, add a block in `config_HFE.h`
and an entry in the `SETS` of the `Makefile`. `include/config_gf2n.h` gives the irreducible
trinomial or pentanomial defining GF(2^n) (`K3 <= 32`) for every `n` in the supported range. The
macros check the combination of parameters at compile time.

**Salt.** With `DRAGON_EXPLICIT_SALT` equal to 0, the salt is not part of the signature, which
therefore has `n` bits. The verifier tries the 256 possible salts, in increasing order, and stops at
the first one for which the signature is valid. With `-DDRAGON_EXPLICIT_SALT=1`, the one-byte salt
is appended to the signature and verification needs a single evaluation.


## Repository layout

```
Makefile
DJames_paper.pdf      the paper
LICENSE
third_party/XKCP/     SHA-3 (git submodule)
include/
  config_HFE.h        parameter sets, sizes, dispatch of the arithmetic on the size of n
  config_gf2n.h       irreducible polynomials of GF(2^n)
  api.h               NIST API
  ...
src/
  sign.c                  NIST API wrappers
  sign_keypairHFE.cpp     key generation
  signHFE.cpp             signing (IP guesses, salt, root selection)
  sign_openHFE.c          verification
  genHFEtab.cpp           random secret data and random invertible matrices
  genSecretMQS.cpp        secret quadratic form: HFE polynomial, IP and Dragon terms
  changeVariableMQS*.c    change of variables by S
  changeEquationMQS.c     mixing of the equations by T
  chooseRootHFE.cpp       roots of the central polynomial (NTL)
  evaluateMQS.cpp         evaluation of the public key
  arith.c, invMatrix.c, genCanonicalBasis.c, initMatrixIdn.c, convNTL.cpp
  rng.c                   NIST DRBG
  bench.c                 benchmark
```

All the sources are compiled as C++.


## Relation to GeMSS

This code is derived from the reference implementation of GeMSS submitted to the NIST
post-quantum standardisation process, written by Antoine Casanova, Jean-Charles Faugère,
Gilles Macario-Rat, Jacques Patarin, Ludovic Perret and Jocelyn Ryckeghem
(<https://polsys.team.lip6.fr/Links/NIST/GeMSS.html>). Most of the finite-field arithmetic (multiplication,
reduction and squaring in GF(2^n), matrix inversion, change of variables, evaluation of the public
key) comes from that implementation.

We removed what James and D-James do not use (vinegar variables, Feistel–Patarin iterations,
encryption, the GeMSS parameter sets) and added the IP modifier, the Dragon terms and the
signing and verification procedures. With matched `n` and degree of the
central polynomial, the running times are comparable to those of GeMSS (Section 7.1 of the paper).
`src/rng.c` and `include/rng.h` are the NIST DRBG (Bassham, NIST). Signing uses NTL's
polynomial factorisation, with NTL's random choices replaced by the DRBG.

Benchmarks against GeMSS (Table 9 of the paper, median of 100 runs):

| Instance (128 / 256 bits) | KeyGen           | Sign               | Verify           |
|---------------------------|------------------|--------------------|------------------|
| D-James                   | 49 ms / 947 ms    | 0.81 ms / 4.36 ms  | 63 µs / 1.36 ms  |
| James                     | 112 ms / 2.22 s   | 1.91 ms / 8.39 ms   | 59 µs / 781 µs  |
| GeMSS                     | 165 ms / 1.9 s   | 3.43 s / 18.1 s    | 94 µs / 865 µs   |
| RedGeMSS                  | 50 ms / 648 ms   | 27.2 ms / 172 ms   | 95 µs / 843 µs   |

The large gap in signing time comes mostly from the degree of the central polynomial: 5 for
James and D-James, 513 for GeMSS and 17 for RedGeMSS.


## Citation

```bibtex
@misc{djames,
  title = {D-James: Ultra Short Multivariate Signatures},
  note  = {Preprint}
}
```


## License

The code written for James and D-James is released under the MIT license (`LICENSE`). XKCP has its own licenses. The files
derived from GeMSS remain subject to the terms of the GeMSS reference implementation.
