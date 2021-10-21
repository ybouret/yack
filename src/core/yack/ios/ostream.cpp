

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

        void ostream:: operator()(const char * , ...)
        {
            throw exception("not implemented");
        }

        void ostream:: put(const uint8_t &x)
        {
            write( char(x) );
        }

        void ostream:: put(const uint16_t &x)
        {
            const uint8_t arr[2] = { (x&0xff), ((x>>8) & 0xff) };
            frame(arr,2);
        }

        void ostream:: put(const uint32_t &x)
        {
            const uint8_t arr[4] = { (x&0xff), ((x>>8) & 0xff), ( (x>>16) & 0xff), ( (x>>24) & 0xff ) };
            frame(arr,4);
        }

        void ostream:: put(const uint64_t &x)
        {
            const uint8_t arr[8] =
            {
                (x&0xff),       ((x>>8) &0xff), ((x>>16)&0xff), ((x>>24)&0xff),
                ((x>>32)&0xff), ((x>>40)&0xff), ((x>>48)&0xff), ((x>>56)&0xff)
            };
            frame(arr,8);
        }

    }

}
