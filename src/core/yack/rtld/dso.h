//! \file
#ifndef YACK_RTLD_DSO_H_INCLUDED
#define YACK_RTLD_DSO_H_INCLUDED 1

#include "yack/system/setup.h"

#if defined(__cplusplus)
#define YACK_DLL_EXTERN() extern "C" {
#define YACK_DLL_FINISH() }
#else
#define YACK_DLL_EXTERN() //!< init exporting zone
#define YACK_DLL_FINISH() //!< quit exporting zone
#endif


#if defined(YACK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define YACK_DLL_API
#define YACK_EXPORT  __declspec(dllexport)
#endif

#if defined(YACK_BSD)
#define YACK_DLL_API
#define YACK_EXPORT
#endif




//______________________________________________________________________________
//
// Specific Module Init/Quit
//______________________________________________________________________________



#if defined(YACK_BSD)

#define YACK_DLL_SETUP(ON_INIT,ON_QUIT)                   \
__attribute__((constructor)) void OnInit() { ON_INIT(); } \
__attribute__((destructor))  void OnQuit() { ON_QUIT(); }

#endif

#if defined(YACK_WIN)

#define YACK_DLL_SETUP(ON_INIT,ON_QUIT)                                  \
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID) \
{                                                                         \
/**/ (void)hinstDLL;                                                      \
/**/ switch( fdwReason )                                                  \
/**/   {                                                                  \
/**/     case DLL_PROCESS_ATTACH: ON_INIT(); break;                       \
/**/     case DLL_PROCESS_DETACH: ON_QUIT(); break;                       \
/**/   }                                                                  \
/**/ return TRUE;                                                         \
}

/*
 case DLL_THREAD_ATTACH:   YACK_dll_flags=1; ON_INIT(); break;\
 case DLL_THREAD_DETACH:   YACK_dll_flags=1; ON_QUIT(); break;\
*/

#endif


#endif


