//! \file

#ifndef YACK_SETUP_INCLUDED
#define YACK_SETUP_INCLUDED 1

#include <assert.h>
#include "yack/system/platform.h"

//______________________________________________________________________________
//
//
//  <STDINT>
//
//______________________________________________________________________________
#include <stdlib.h>
#include <stddef.h>
#if defined(YACK_STDINT)
# undef YACK_STDINT
#endif

//______________________________________________________________________________
//
//    <GNUC>
//______________________________________________________________________________
#if defined(__GNUC__)
#define YACK_STDINT 1
#include <stdint.h>
#    define    YACK_U64(X) X##ULL
#    define    YACK_I64(X) X##LL
#endif
//______________________________________________________________________________
//
//    <GNUC/>
//______________________________________________________________________________


//______________________________________________________________________________
//
//    <Microsoft>
//______________________________________________________________________________
#if defined(_MSC_VER)
#if _MSC_VER >= 1900
#include <stdint.h>
#else
// Microsoft built-in types
typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
#endif

#        define    YACK_STDINT 1
#        define    YACK_U64(X) X##ui64
#        define    YACK_I64(X) X##i64

#endif
//______________________________________________________________________________
//
//    <Microsoft/>
//______________________________________________________________________________

#if !defined(YACK_STDINT)
#    error "no STDINT"
#endif

//______________________________________________________________________________
//
//
//  <STDINT/>
//
//______________________________________________________________________________



#if defined(__cplusplus)
extern "C" {
#endif

    extern const char yack_unknown[];         //!< "unknown"
    extern const char yack_success[];         //!< "success"
    extern const char yack_failure[];         //!< "failure"
    int               yack_die(const char *); //!< return 1
    int               yack_good(const void *addr, const size_t size); //!< !(addr==NULL&&size>0)
#if defined(__cplusplus)
}
#endif

#endif

