
#include "yack/system/imported.hpp"
#include <cstring>
#include <cstdarg>

namespace yack
{

    namespace imported
    {
        void  exception:: erase() throw()
        {
            memset(text,0,sizeof(text));
        }

        exception:: ~exception() throw()
        {
            erase();
        }

        exception:: exception() throw() :
        yack::exception(),
        text()
        {
            erase();
        }

        exception:: exception(const exception &other) throw() :
        yack::exception(other),
        text()
        {
            memcpy(text,other.text,sizeof(text));
        }

        const char * exception:: what() const throw()
        {
            return text;
        }

        void exception:: describe(const char *fmt,...) throw()
        {
            va_list ap;
            va_start(ap,fmt);
            failsafe_format(text,sizeof(text),fmt,&ap);
            va_end(ap);
        }
    }

}

