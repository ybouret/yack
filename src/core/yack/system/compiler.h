
//! \file

#ifndef YACK_COMPILER_INCLUDED
#define YACK_COMPILER_INCLUDED 1

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
#define YACK_MICROSOFT 1
#define YACK_COMPILER "microsoft"
#endif

#if defined(__ICC)
#define YACK_INTEL 1
#define YACK_COMPILER "intel"
#endif


#if !defined(YACK_COMPILER)
#error "YACK_COMPILER was not found"
#endif

#endif


