

#include "yack/ios/ostream.hpp"
#include "yack/system/exception.hpp"
#include "yack/type/cstring.h"
#include "yack/memory/buffers.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/string.hpp"

#include <iostream>
#include <cerrno>
#include <cstdarg>
#include <cstdio>

namespace yack
{
    namespace ios
    {

        ostream:: ~ostream() noexcept
        {
        }


        ostream:: ostream() noexcept
        {
        }

        void ostream:: frame(const void *addr, const size_t size)
        {
            const char *ptr = static_cast<const char *>(addr);
            for(size_t i=size;i>0;--i)
            {
                write(*(ptr++));
            }
        }

        ostream & ostream:: operator<<(const char C)
        {
            write(C);
            return *this;
        }

        ostream & ostream:: operator<<(const char *msg)
        {
            frame(msg,yack_cstring_size(msg));
            return *this;
        }

        ostream & ostream:: operator<<(const string &str)
        {
            frame(str(),str.size());
            return *this;
        }

        size_t ostream:: operator()(const char *fmt,...)
        {
            assert(NULL!=fmt);
            int res = 0;
            {
                va_list ap;
                va_start(ap,fmt);
                res = vsnprintf(NULL,0,fmt,ap);
                va_end(ap);
            }
            if(res>0)
            {
                memory::buffer_of<char,memory::pooled> buff(res+1);
                {
                    va_list ap;
                    va_start(ap,fmt);
                    const int res2 = vsnprintf(*buff,buff.size(),fmt,ap);
                    va_end(ap);
                    if(res2!=res) throw libc::exception(EINVAL,"ostream:: vsnprintf failure");
                }
                frame(*buff,res);
            }

            return size_t(res);
        }



    }

}
