
#include "yack/system/imported.hpp"
#include <cstring>
#include <cstdarg>
#include "yack/type/cstring.h"

namespace yack
{

    namespace imported
    {
        void  exception:: erase() noexcept
        {
            memset(text,0,sizeof(text));
        }

        exception:: ~exception() noexcept
        {
            erase();
        }

        exception:: exception() noexcept :
        yack::exception(),
        text()
        {
            erase();
        }

        exception:: exception(const exception &other) noexcept :
        yack::exception(other),
        text()
        {
            memcpy(text,other.text,sizeof(text));
        }

        const char * exception:: what() const noexcept
        {
            return text;
        }

        void exception:: describe(const char *fmt,...) noexcept
        {
            va_list ap;
            va_start(ap,fmt);
            failsafe_format(text,sizeof(text),fmt,&ap);
            va_end(ap);
        }

        exception:: exception(const char *where, const char *fmt,...) noexcept :
        yack::exception(),
        text()
        {
            erase();
            yack_cstring_msgcpy(text,sizeof(text),where);
            
            va_list ap;
            va_start(ap,fmt);
            failsafe_format(info,sizeof(info),fmt,&ap);
            va_end(ap);
        }
    }

}

