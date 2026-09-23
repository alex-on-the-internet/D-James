#ifndef _CHANGEEQUATIONMQS_H
#define _CHANGEEQUATIONMQS_H


#include "config_HFE.h"
#include "types_HFE.h"

/* Computes the public key pk (m equations) from the secret quadratic form
   MQS and the matrix T. Returns ERROR_ALLOC if an allocation fails. */
int changeEquationMQS(pk_gf2 pk, cst_mqsn_gf2n MQS, cst_Mn_gf2 T);


#endif
