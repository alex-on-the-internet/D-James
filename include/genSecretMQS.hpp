#ifndef _GENSECRETMQS_HPP
#define _GENSECRETMQS_HPP

#include "config_HFE.h"

/* Builds the secret quadratic form directly in the compact mqsn_gf2n
   layout from the secret key F2 (see genSecretMQS.cpp). No NTL or other
   high-level object is used. */
int genSecretMQS(mqsn_gf2n Q, const UINT* F2);

#endif
