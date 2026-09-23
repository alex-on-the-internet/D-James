#ifndef _BIT_H
#define _BIT_H

/* Macros on the bits of 64-bit words and on vectors of words */

/* (2^k) - 1, k<64, and -1 for k=0 */
#define mask64(k) ((k)?(1UL<<(k))-1UL:-1UL)

/* Parity of the 64 bits of n */
#define XORBITS64(n) \
    n^=n >> 32U;\
    n^=n >> 16U;\
    n^=n >> 8U;\
    n^=n >> 4U;\
    n^=n >> 2U;\
    n^=n >> 1U;\
    n&=1UL;


/* Dot product of two vectors of bits */
#define DOTPRODUCT128(res,a,b) \
    res=(a)[0]&(b)[0];\
    res^=(a)[1]&(b)[1];\
    XORBITS64(res);

#define DOTPRODUCT192(res,a,b) \
    res=(a)[0]&(b)[0];\
    res^=(a)[1]&(b)[1];\
    res^=(a)[2]&(b)[2];\
    XORBITS64(res);

#define DOTPRODUCT256(res,a,b) \
    res=(a)[0]&(b)[0];\
    res^=(a)[1]&(b)[1];\
    res^=(a)[2]&(b)[2];\
    res^=(a)[3]&(b)[3];\
    XORBITS64(res);

#define DOTPRODUCT320(res,a,b) \
    res=(a)[0]&(b)[0];\
    res^=(a)[1]&(b)[1];\
    res^=(a)[2]&(b)[2];\
    res^=(a)[3]&(b)[3];\
    res^=(a)[4]&(b)[4];\
    XORBITS64(res);

#define DOTPRODUCT384(res,a,b) \
    res=(a)[0]&(b)[0];\
    res^=(a)[1]&(b)[1];\
    res^=(a)[2]&(b)[2];\
    res^=(a)[3]&(b)[3];\
    res^=(a)[4]&(b)[4];\
    res^=(a)[5]&(b)[5];\
    XORBITS64(res);

#define DOTPRODUCT448(res,a,b) \
    res=(a)[0]&(b)[0];\
    res^=(a)[1]&(b)[1];\
    res^=(a)[2]&(b)[2];\
    res^=(a)[3]&(b)[3];\
    res^=(a)[4]&(b)[4];\
    res^=(a)[5]&(b)[5];\
    res^=(a)[6]&(b)[6];\
    XORBITS64(res);

#define DOTPRODUCT512(res,a,b) \
    res=(a)[0]&(b)[0];\
    res^=(a)[1]&(b)[1];\
    res^=(a)[2]&(b)[2];\
    res^=(a)[3]&(b)[3];\
    res^=(a)[4]&(b)[4];\
    res^=(a)[5]&(b)[5];\
    res^=(a)[6]&(b)[6];\
    res^=(a)[7]&(b)[7];\
    XORBITS64(res);

#define DOTPRODUCT576(res,a,b) \
    res=(a)[0]&(b)[0];\
    res^=(a)[1]&(b)[1];\
    res^=(a)[2]&(b)[2];\
    res^=(a)[3]&(b)[3];\
    res^=(a)[4]&(b)[4];\
    res^=(a)[5]&(b)[5];\
    res^=(a)[6]&(b)[6];\
    res^=(a)[7]&(b)[7];\
    res^=(a)[8]&(b)[8];\
    XORBITS64(res);

#define DOTPRODUCT640(res,a,b) \
    res=(a)[0]&(b)[0];\
    res^=(a)[1]&(b)[1];\
    res^=(a)[2]&(b)[2];\
    res^=(a)[3]&(b)[3];\
    res^=(a)[4]&(b)[4];\
    res^=(a)[5]&(b)[5];\
    res^=(a)[6]&(b)[6];\
    res^=(a)[7]&(b)[7];\
    res^=(a)[8]&(b)[8];\
    res^=(a)[9]&(b)[9];\
    XORBITS64(res);


/* res ^= a & b, with b a constant word */
#define DOTPRODUCT64M(res,a,b) \
    res[0]^=(a)[0]&(b);

#define DOTPRODUCT128M(res,a,b) \
    DOTPRODUCT64M(res,a,b);\
    res[1]^=(a)[1]&(b);

#define DOTPRODUCT192M(res,a,b) \
    DOTPRODUCT128M(res,a,b);\
    res[2]^=(a)[2]&(b);

#define DOTPRODUCT256M(res,a,b) \
    DOTPRODUCT192M(res,a,b);\
    res[3]^=(a)[3]&(b);

#define DOTPRODUCT320M(res,a,b) \
    DOTPRODUCT256M(res,a,b);\
    res[4]^=(a)[4]&(b);

#define DOTPRODUCT384M(res,a,b) \
    DOTPRODUCT320M(res,a,b);\
    res[5]^=(a)[5]&(b);

#define DOTPRODUCT448M(res,a,b) \
    DOTPRODUCT384M(res,a,b);\
    res[6]^=(a)[6]&(b);

#define DOTPRODUCT512M(res,a,b) \
    DOTPRODUCT448M(res,a,b);\
    res[7]^=(a)[7]&(b);

#define DOTPRODUCT576M(res,a,b) \
    DOTPRODUCT512M(res,a,b);\
    res[8]^=(a)[8]&(b);

#define DOTPRODUCT640M(res,a,b) \
    DOTPRODUCT576M(res,a,b);\
    res[9]^=(a)[9]&(b);



/* Equal 0 */
#define ISZERO64(a) ((a)[0]==0UL)
#define ISZERO128(a) (ISZERO64(a)&&((a)[1]==0UL))
#define ISZERO192(a) (ISZERO128(a)&&((a)[2]==0UL))
#define ISZERO256(a) (ISZERO192(a)&&((a)[3]==0UL))
#define ISZERO320(a) (ISZERO256(a)&&((a)[4]==0UL))
#define ISZERO384(a) (ISZERO320(a)&&((a)[5]==0UL))
#define ISZERO448(a) (ISZERO384(a)&&((a)[6]==0UL))
#define ISZERO512(a) (ISZERO448(a)&&((a)[7]==0UL))
#define ISZERO576(a) (ISZERO512(a)&&((a)[8]==0UL))
#define ISZERO640(a) (ISZERO576(a)&&((a)[9]==0UL))



/* Equality */
#define ISEQUAL64(a,b) ((a)[0]==(b)[0])
#define ISEQUAL128(a,b) (ISEQUAL64(a,b)&&((a)[1]==(b)[1]))
#define ISEQUAL192(a,b) (ISEQUAL128(a,b)&&((a)[2]==(b)[2]))
#define ISEQUAL256(a,b) (ISEQUAL192(a,b)&&((a)[3]==(b)[3]))
#define ISEQUAL320(a,b) (ISEQUAL256(a,b)&&((a)[4]==(b)[4]))
#define ISEQUAL384(a,b) (ISEQUAL320(a,b)&&((a)[5]==(b)[5]))
#define ISEQUAL448(a,b) (ISEQUAL384(a,b)&&((a)[6]==(b)[6]))
#define ISEQUAL512(a,b) (ISEQUAL448(a,b)&&((a)[7]==(b)[7]))
#define ISEQUAL576(a,b) (ISEQUAL512(a,b)&&((a)[8]==(b)[8]))
#define ISEQUAL640(a,b) (ISEQUAL576(a,b)&&((a)[9]==(b)[9]))



/* Comparison */
#define CMP_LT64(a,b) ((a)[0]<(b)[0])
#define CMP_LT128(a,b) (((a)[1]==(b)[1])?CMP_LT64(a,b):((a)[1]<(b)[1]))
#define CMP_LT192(a,b) (((a)[2]==(b)[2])?CMP_LT128(a,b):((a)[2]<(b)[2]))
#define CMP_LT256(a,b) (((a)[3]==(b)[3])?CMP_LT192(a,b):((a)[3]<(b)[3]))
#define CMP_LT320(a,b) (((a)[4]==(b)[4])?CMP_LT256(a,b):((a)[4]<(b)[4]))
#define CMP_LT384(a,b) (((a)[5]==(b)[5])?CMP_LT320(a,b):((a)[5]<(b)[5]))
#define CMP_LT448(a,b) (((a)[6]==(b)[6])?CMP_LT384(a,b):((a)[6]<(b)[6]))
#define CMP_LT512(a,b) (((a)[7]==(b)[7])?CMP_LT448(a,b):((a)[7]<(b)[7]))
#define CMP_LT576(a,b) (((a)[8]==(b)[8])?CMP_LT512(a,b):((a)[8]<(b)[8]))
#define CMP_LT640(a,b) (((a)[9]==(b)[9])?CMP_LT576(a,b):((a)[9]<(b)[9]))

#define CMP_GT64(a,b) ((a)[0]>(b)[0])
#define CMP_GT128(a,b) (((a)[1]==(b)[1])?CMP_GT64(a,b):((a)[1]>(b)[1]))
#define CMP_GT192(a,b) (((a)[2]==(b)[2])?CMP_GT128(a,b):((a)[2]>(b)[2]))
#define CMP_GT256(a,b) (((a)[3]==(b)[3])?CMP_GT192(a,b):((a)[3]>(b)[3]))
#define CMP_GT320(a,b) (((a)[4]==(b)[4])?CMP_GT256(a,b):((a)[4]>(b)[4]))
#define CMP_GT384(a,b) (((a)[5]==(b)[5])?CMP_GT320(a,b):((a)[5]>(b)[5]))
#define CMP_GT448(a,b) (((a)[6]==(b)[6])?CMP_GT384(a,b):((a)[6]>(b)[6]))
#define CMP_GT512(a,b) (((a)[7]==(b)[7])?CMP_GT448(a,b):((a)[7]>(b)[7]))
#define CMP_GT576(a,b) (((a)[8]==(b)[8])?CMP_GT512(a,b):((a)[8]>(b)[8]))
#define CMP_GT640(a,b) (((a)[9]==(b)[9])?CMP_GT576(a,b):((a)[9]>(b)[9]))

#endif
