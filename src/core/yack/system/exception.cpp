#include "yack/system/exception.hpp"
#include <cstring>
#include <cstdarg>
#include "yack/system/error.hpp"

namespace yack
{
    namespace libc
    {
        exception:: ~exception() noexcept
        {
        }

        exception:: exception(const exception &other) noexcept :
        excp_type(other)
        {
        }


        exception:: exception(const int err, const char *fmt,...) noexcept :
        excp_type(err)
        {
            {
                va_list ap;
                va_start(ap,fmt);
                failsafe_format(info,sizeof(info),fmt,&ap);
                va_end(ap);
            }

           system_error::format_bsd(text,sizeof(text),code);
        }
        
    }

}


#if defined(YACK_DARWIN)
#include <mach/mach.h>
#include "yack/type/cstring.h"

namespace yack
{
    namespace mach
    {
        exception:: ~exception() noexcept
        {
        }

        exception:: exception(const exception &other) noexcept :
        excp_type(other)
        {
        }


        exception:: exception(const int err, const char *fmt,...) noexcept :
        excp_type(err)
        {
            {
                va_list ap;
                va_start(ap,fmt);
                failsafe_format(info,sizeof(info),fmt,&ap);
                va_end(ap);
            }

            yack_cstring_msgcpy(text,sizeof(text),mach_error_string(err));
        }

    }

}
#endif


#if defined(YACK_WIN)
namespace yack
{
    namespace win32
    {
        exception:: ~exception() noexcept
        {
        }

        exception::exception(const exception &other) noexcept :
        excp_type(other)
        {
        }


        exception::exception(const uint32_t err, const char *fmt, ...) noexcept :
        excp_type(err)
        {
            {
                va_list ap;
                va_start(ap, fmt);
                failsafe_format(info, sizeof(info), fmt, &ap);
                va_end(ap);
            }

            system_error::format_win(text, sizeof(text), code);
        }

    }

}

#endif
