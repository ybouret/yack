
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
#define YACK_MAJOR __GNUC__
#define YACK_MINOR __GNUC_MINOR__
#define YACK_PATCH __GNUC_PATCHLEVEL__
#endif

#if defined(__clang__)
#define YACK_CLANG 1
#define YACK_COMPILER "clang"
#define YACK_MAJOR __clang_major__
#define YACK_MINOR __clang_minor__
#define YACK_PATCH __clang_patchlevel__
#endif

#if defined(_MSC_VER)
#define YACK_MICROSOFT 1
#define YACK_COMPILER "microsoft"
#define YACK_MAJOR __MSC_VER__
#define YACK_MINOR 0
#define YACK_PATCH 0
#endif

#if defined(__ICC)
#define YACK_INTEL 1
#define YACK_COMPILER "intel"
#define YACK_MAJOR __ICC
#define YACK_MINOR 0
#define YACK_PATCH 0
#endif


#if !defined(YACK_COMPILER)
#error "YACK_COMPILER was not found"
#endif

#endif


