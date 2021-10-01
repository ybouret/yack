
//! \file

#ifndef YACK_PLATFORM_INCLUDED
#define YACK_PLATFORM_INCLUDED 1

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
#define YACK_DARWIN 1
#define YACK_BSD   1
#define YACK_PLATFORM "Darwin"
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



#endif

