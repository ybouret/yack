

#include "yack/ios/ostream.hpp"
#include "yack/exception.hpp"
#include "yack/type/cstring.h"
namespace yack
{
    namespace ios
    {

        ostream:: ~ostream() throw()
        {
        }


        ostream:: ostream() throw()
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

        size_t ostream:: operator()(const char *fmt,...)
        {
            throw exception("not implemented");
        }

      

    }

}
