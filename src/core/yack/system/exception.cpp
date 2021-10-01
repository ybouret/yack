#include "yack/system/exception.hpp"
#include <cstring>
#include <cstdarg>
#include "yack/system/error.hpp"

namespace yack
{
    namespace libc
    {
        exception:: ~exception() throw()
        {
        }

        exception:: exception(const exception &other) throw() :
        excp_type(other)
        {
        }


        exception:: exception(const int err, const char *fmt,...) throw() :
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

#if defined(YACK_WIN)
namespace yack
{
    namespace win32
    {
        exception:: ~exception() throw()
        {
        }

        exception::exception(const exception &other) throw() :
        excp_type(other)
        {
        }


        exception::exception(const uint32_t err, const char *fmt, ...) throw() :
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
