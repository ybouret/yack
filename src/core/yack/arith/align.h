
//! \file

#ifndef YACK_ALIGN_INCLUDED
#define YACK_ALIGN_INCLUDED 1

#include "yack/system/setup.h"

#define YACK_SIZE_T(X) ( (size_t)(X) )
#define YACK_SIZE_1    YACK_SIZE_T(1)

//! get align byte=2^LN2
#define YACK_ALIGN_BYTE( LN2 ) YACK_SIZE_T(YACK_SIZE_1<<(LN2))
 
//! get align mask=(2^LN2-1)
#define YACK_ALIGN_MASK( LN2 ) YACK_SIZE_T( YACK_ALIGN_BYTE(LN2) - YACK_SIZE_1 )
 
//! get rounding mask 2-complement
#define YACK_ALIGN_NOT2(LN2)   YACK_SIZE_T( ~YACK_ALIGN_MASK(LN2) )

//! get the rounded value
#define YACK_ALIGN_LN2(LN2,N) ( ( YACK_SIZE_T(N) + YACK_ALIGN_MASK(LN2) ) & YACK_ALIGN_NOT2(LN2) )


#endif
