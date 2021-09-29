#include "yack/system/exception.hpp"
#include <cstring>

namespace yack
{

    void system_exception:: erase() throw()
    {
        memset(text,0,sizeof(text));
    }

    system_exception:: ~system_exception() throw()
    {
        erase();
    }

    system_exception:: system_exception() throw() :
    exception(),
    text()
    {
        erase();
    }

    system_exception:: system_exception(const system_exception &other) throw() :
    exception(other),
    text()
    {
        memcpy(text,other.text,sizeof(text));
    }

    const char * system_exception:: what() const throw()
    {
        return text;
    }

}

#include <cstdarg>
#include "yack/system/error.h"

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

            yack_bsd_format_error(text,sizeof(text),code);
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

            yack_win_format_error(text, sizeof(text), code);
        }

    }

}

#endif
