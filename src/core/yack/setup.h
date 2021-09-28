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

#if defined(__GNUC__) && !defined(__clang__)
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
// integral types
//
//______________________________________________________________________________

#include <stdlib.h>

#if defined(Y_MSC)

#else
#include <inttypes.h>
#endif

#endif

