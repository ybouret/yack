
//! \file

#ifndef YACK_ALIGN_INCLUDED
#define YACK_ALIGN_INCLUDED 1

#include "yack/arith/ilog2.hpp"


#define YACK_SIZE_T(X) ( (size_t)(X) ) //!< in-place size_t
#define YACK_SIZE_1    YACK_SIZE_T(1)  //!< in-place 1 as size_t

//! get align byte=2^LN2
#define YACK_ALIGN_BYTE( LN2 ) YACK_SIZE_T(YACK_SIZE_1<<(LN2))
 
//! get align mask=(2^LN2-1)
#define YACK_ALIGN_MASK( LN2 ) YACK_SIZE_T( YACK_ALIGN_BYTE(LN2) - YACK_SIZE_1 )
 
//! get rounding mask 2-complement
#define YACK_ALIGN_NOT2(LN2)   YACK_SIZE_T( ~YACK_ALIGN_MASK(LN2) )

//! get the rounded value
#define YACK_ALIGN_LN2(LN2,N) ( ( YACK_SIZE_T(N) + YACK_ALIGN_MASK(LN2) ) & YACK_ALIGN_NOT2(LN2) )

#define YACK_ALIGN(N)    YACK_ALIGN_LN2(0,N) //!< align on 1 byte   | 8 bits
#define YACK_ALIGN2(N)   YACK_ALIGN_LN2(1,N) //!< align on 2 bytes  | 16 bits
#define YACK_ALIGN4(N)   YACK_ALIGN_LN2(2,N) //!< align on 4 bytes  | 32 bits
#define YACK_ALIGN8(N)   YACK_ALIGN_LN2(3,N) //!< align on 8 bytes  | 64 bits
#define YACK_ALIGN16(N)  YACK_ALIGN_LN2(4,N) //!< align on 16 bytes | 128 bits

#define YACK_MEMALIGN(N) YACK_ALIGN16(N)    //!< default memory alignment

//! align on power of two SIZE
#define YACK_ALIGN_ON(SIZE,N) YACK_ALIGN_LN2(yack::ilog2<SIZE>::value,N)

//! align on sizeof(TYPE) = power of two
#define YACK_ALIGN_TO(TYPE,N) YACK_ALIGN_ON(sizeof(TYPE),N)

//! number of words to hold TYPE
#define YACK_WORDS_FOR(TYPE)  (YACK_ALIGN_TO(void*,sizeof(TYPE))/sizeof(void*))


#endif
