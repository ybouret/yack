#include "yack/exception.hpp"
#include "yack/type/cstring.h"
#include "yack/type/utils.hpp"
#include <cstring>
#include <cstdarg>
#include <iostream>

namespace yack
{

    const char exception:: what_text[] = "yack::exception";

    void exception:: clear() noexcept
    {
        memset(info,0,sizeof(info));
    }

    exception:: ~exception() noexcept
    {
        clear();
    }

    exception:: exception() noexcept :
    std::exception(),
    info()
    {
        clear();
    }


    
    void exception:: failsafe_format(char *buffer, size_t buflen, const char *fmt, void *args) noexcept
    {
        assert(NULL!=buffer);
        assert(0<buflen);
        assert(NULL!=fmt);
        assert(NULL!=args);
        
        va_list &ap = *static_cast<va_list *>(args);
        if(!yack_cstring_format(buffer,buflen,fmt,&ap))
        {
            yack_cstring_msgcpy(buffer,buflen,"exception: format failure...");
        }
    }

    exception:: exception(const char *fmt,...) noexcept :
    std::exception(),
    info()
    {
        clear();
        va_list ap;
        va_start(ap,fmt);
        failsafe_format(info,sizeof(info),fmt,&ap);
        va_end(ap);
    }

    exception:: exception(const exception &other) noexcept :
    std::exception(),
    info()
    {
        memcpy(info,other.info,sizeof(info));
    }

    const char * exception:: when() const noexcept
    {
        return info;
    }

    const char * exception:: what() const noexcept
    {
        return what_text;
    }

    exception & exception:: operator<<(const char *msg) noexcept
    {
        yack_cstring_msgcat(info,sizeof(info),msg);
        return *this;
    }

    exception & exception:: operator<<(const char c) noexcept
    {
        yack_cstring_chrcat(info,sizeof(info),c);
        return *this;
    }

    exception & exception:: add(const char *fmt,...) noexcept
    {
        char    data[when_size] = { 0 };
        {
            va_list ap;
            va_start(ap,fmt);
            failsafe_format(data,sizeof(data),fmt,&ap);
            va_end(ap);
        }

        yack_cstring_msgcat(info,sizeof(info),data);


        return *this;
    }


    exception & exception:: pre(const char *fmt,...) noexcept
    {
        char    data[when_size] = { 0 };
        {
            va_list ap;
            va_start(ap,fmt);
            failsafe_format(data,sizeof(data),fmt,&ap);
            va_end(ap);
        }

        yack_cstring_msgcat(data,sizeof(data),info);
        memcpy(info,data,when_size);

        return *this;
    }


    void exception:: show(std::ostream &os) const
    {
        const size_t what_size = strlen(what());
        const size_t when_size = strlen(when());
        const size_t full_size = max_of(what_size,when_size);
        const size_t width     = 2+full_size;

        os << '/'; for(size_t i=width;i>0;--i) os << '-'; os << '\\' << std::endl;
        os << '|' << ' ' << what(); for(size_t i=what_size;i<full_size;++i) os << ' '; os << ' ' << '|' << std::endl;
        os << '|' << ' ' << when(); for(size_t i=when_size;i<full_size;++i) os << ' '; os << ' ' << '|' << std::endl;
        os << '\\'; for(size_t i=width;i>0;--i) os << '-'; os << '/' << std::endl;

    }

}
