#include "arith.h"
#include "mul.h"
#include "rem.h"


#if HFEnr
    #define INIT_Q(size2) uint64_t Q[size2]
#else
    /* Q is not needed by the reduction */
    #define INIT_Q(size2)
#endif

#define MUL_MOD_MACRO(name_function,mul_function,rem_function,size,size2) name_function\
{ \
    uint64_t res_mul[size],R; \
    INIT_Q(size2); \
    mul_function; \
    rem_function; \
}

MUL_MOD_MACRO(HFEMULMOD,hfemul(res_mul,A,B),HFEREM(res,res_mul,Q,R),NB_WORD_MUL,NB_WORD_GFqn);
MUL_MOD_MACRO(HFESQUAREMOD,hfesquare(res_mul,A),HFEREM(res,res_mul,Q,R),NB_WORD_MUL,NB_WORD_GFqn);
