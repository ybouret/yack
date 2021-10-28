//! \file
#ifndef YACK_FOURCC_INCLUDED
#define YACK_FOURCC_INCLUDED

#include "yack/system/setup.h"

#if defined(__cplusplus)
extern "C" {
#endif

    //! byte
#define YACK_FOURCC0(VALUE) ((uint32_t)((uint8_t)(VALUE)))
    //! shifted byte
#define YACK_FOURCC1(VALUE) ( YACK_FOURCC0(VALUE) << 0x08 )
    //! shifted byte
#define YACK_FOURCC2(VALUE) ( YACK_FOURCC0(VALUE) << 0x10 )
    //! shifted byte
#define YACK_FOURCC3(VALUE) ( YACK_FOURCC0(VALUE) << 0x18 )

    //! Four Characters Code
#define YACK_FOURCC(A,B,C,D) (YACK_FOURCC3(A) | YACK_FOURCC2(B) | YACK_FOURCC1(C) | YACK_FOURCC0(D))

    //! shr for index
#define YACK_FOURCC_SHR(INDX) ((unsigned)((3-(INDX)) << 3))
#define YACK_FOURCC_U32(VALUE) ( (uint32_t)(VALUE) )

    //! extracting byte from a four chars code
#define YACK_FOURCC_AT(INDX,DWORD) ((uint8_t)(YACK_FOURCC_U32(DWORD) >> YACK_FOURCC_SHR(INDX)))

    //! convert a dword into an array of 8 chars
#define YACK_FOURCC_CHAR8(DWORD) {      \
(char)(YACK_FOURCC_AT(0,DWORD)),        \
(char)(YACK_FOURCC_AT(1,DWORD)),        \
(char)(YACK_FOURCC_AT(2,DWORD)),        \
(char)(YACK_FOURCC_AT(3,DWORD)),        \
0,0,0,0 }

    //! thread unsafe conversion
    const char *yack_fourcc(const uint32_t code);
    
#if defined(__cplusplus)
}
#endif

#endif


