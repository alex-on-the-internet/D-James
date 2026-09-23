#ifndef _EVALUATEMQS_HPP
#define _EVALUATEMQS_HPP

#include "config_HFE.h"

/* c = pk(m): evaluates the public key on HFEN bits (HFEN==HFEn for James,
   x||Y for D-James); c has HFEm bits. */
void evaluateMQS_pk(vecm_gf2 c, cst_vecN_gf2 m, cst_pk_gf2 pk);

#endif
