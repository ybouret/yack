
//! \file

#ifndef YACK_ARCH_INCLUDED
#define YACK_ARCH_INCLUDED 1


#include "yack/system/compiler.h"

//______________________________________________________________________________
//
//
// finding out YACK_ARCHITECTURE
// ix386
// amd64
//______________________________________________________________________________

#if defined(YACK_ARCHITECTURE)
#error "YACK_ARCHITECTURE shouldn't be defined here!!"
#endif

#if defined(YACK_GNU) || defined(YACK_CLANG) || defined(YACK_INTEL)

#if defined(__i386__)
#define YACK_ARCHITECTURE "i386"
#define YACK_NATIVE_BITS   32
#endif

#if defined(__x86_64__)
#define YACK_ARCHITECTURE "amd64"
#define YACK_NATIVE_BITS     64
#endif

#endif

#if defined(YACK_MICROSOFT)

#idef defined(_M_IX86)
#define YACK_ARCHITECTURE "i386"
#define YACK_NATIVE_BITS     32
#endif

#if defined(_M_AMD64)
#define YACK_ARCHITECTURE "amd64"
#define YACK_NATIVE_BITS     64
#endif




#if !defined(YACK_ARCHITECTURE)
#error "YACK_ARCHITECTURE was not found"
#endif

#if !defined(YACK_NATIVE_BITS)
#error "YACK_NATIVE_BITS was not found"
#endif

#endif
