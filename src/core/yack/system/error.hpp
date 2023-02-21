//! \file

#ifndef YACK_SYSTEM_ERROR_INCLUDED
#define YACK_SYSTEM_ERROR_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{

#if defined(YACK_BSD)
    typedef int      system_error_t; //!< alias
#endif

#if defined(YACK_WIN)
    typedef uint32_t system_error_t; //!< alias
#endif

    //__________________________________________________________________________
    //
    //
    //! wrappers for system errors
    //
    //__________________________________________________________________________
    struct system_error
    {
        //! invalid value
        static const system_error_t invalid;

        //! error from strerror
        static void format_bsd(char *buffer, const size_t length, const int err) noexcept;

        //! emit error and exit
        static void critical_bsd(const int res, const char *ctx) noexcept;


#if defined(YACK_WIN)
        //! error form FormatMessage
        static void format_win(char *buffer, const size_t length, const uint32_t err) noexcept;
        static void critical_win(const uint32_t err, const char *ctx) noexcept;
#endif

        //! helper for never_get_here code
        static  void never_get_here(const char *ctx) noexcept;

    };

#if defined(YACK_WIN)
#define YACK_SYSTEM_ERROR_FORMAT yack::system_error::format_win
#endif

#if defined(YACK_BSD)
#define YACK_SYSTEM_ERROR_FORMAT yack::system_error::format_bsd
#endif




}

#endif

