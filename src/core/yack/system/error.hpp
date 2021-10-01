//! \file

#ifndef YACK_SYSTEM_ERROR_INCLUDED
#define YACK_SYSTEM_ERROR_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{

    struct system_error
    {
        //! error from strerror
        static void format_bsd(char *buffer, const size_t length, const int err) throw();

        //! error form FormatMessage
#if defined(YACK_WIN)
        static void format_win(char *buffer, const size_t length, const uint32_t err) throw();
#endif
        
        static void critical_bsd(const int res, const char *ctx) throw();
    };

}

#endif

