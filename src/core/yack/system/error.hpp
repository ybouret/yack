//! \file

#ifndef YACK_SYSTEM_ERROR_INCLUDED
#define YACK_SYSTEM_ERROR_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! wrappers for system errors
    //
    //__________________________________________________________________________
    struct system_error
    {
        //! error from strerror
        static void format_bsd(char *buffer, const size_t length, const int err) throw();
      
        //! emit error and exit
        static void critical_bsd(const int res, const char *ctx) throw();

#if defined(YACK_WIN)
        //! error form FormatMessage
        static void format_win(char *buffer, const size_t length, const uint32_t err) throw();
        static void critical_win(const uint32_t err, const char *ctx) throw();
#endif
        
      
    };

}

#endif

