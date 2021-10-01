//! \file

#ifndef YACK_SETUP_INCLUDED
#define YACK_SETUP_INCLUDED 1

#include <assert.h>

//______________________________________________________________________________
//
//
// finding out YACK_PLATFORM
//
//______________________________________________________________________________

#if defined(YACK_PLATFORM)
#error "YACK_PLATFORM shouldn't be defined here!!"
#endif

#if defined(__APPLE__)
#define YACK_APPLE 1
#define YACK_BSD   1
#define YACK_PLATFORM "Apple"
#endif

#if defined(__FreeBSD__)
#define YACK_FREEBSD 1
#define YACK_BSD     1
#define YACK_PLATFORM "FreeBSD"
#endif

#if defined(__linux__)
#define YACK_LINUX 1
#define YACK_BSD   1
#define YACK_PLATFORM "Linux"
#endif

#if defined(__OpenBSD__)
#define YACK_OPENBSD 1
#define YACK_BSD     1
#define YACK_PLATFORM "OpenBSD"
#endif

#if defined(__sun__)
#define YACK_SUNOS   1
#define YACK_BSD     1
#define YACK_PLATFORM "SunOSD"
#endif

#if defined(_WIN32)
#define YACK_WIN     1
#define YACK_PLATFORM "Windows"
#endif

#if !defined(YACK_PLATFORM)
#error "YACK_PLATFORM was not found"
#endif


//______________________________________________________________________________
//
//
// finding out YACK_COMPILER
//
//______________________________________________________________________________

#if defined(YACK_COMPILER)
#error "YACK_COMPILER shouldn't be defined here!!"
#endif

#if defined(__GNUC__) && !defined(__clang__) && !defined(__ICC)
#define YACK_GNU 1
#define YACK_COMPILER "gnu"
#endif

#if defined(__clang__)
#define YACK_CLANG 1
#define YACK_COMPILER "clang"
#endif

#if defined(_MSC_VER)
#define YACK_MSC 1
#define YACK_COMPILER "msc"
#endif

#if defined(__ICC)
#define YACK_INTEL 1
#define YACK_COMPILER "intel"
#endif


#if !defined(YACK_COMPILER)
#error "YACK_COMPILER was not found"
#endif

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


#if defined(__cplusplus)
extern "C" {
#endif

    extern const char yack_unknown[];         //!< "unknown"
    extern const char yack_success[];         //!< "success"
    extern const char yack_failure[];         //!< "failure"
    int               yack_die(const char *); //!< return 1
    
#if defined(__cplusplus)
}
#endif

#endif

