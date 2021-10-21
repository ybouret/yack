

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
            const uint8_t arr[2] = { uint8_t(x&0xff), uint8_t((x>>8) & 0xff) };
            frame(arr,2);
        }

        void ostream:: put(const uint32_t &x)
        {
            const uint8_t arr[4] = { uint8_t(x&0xff), uint8_t((x>>8) & 0xff), uint8_t( (x>>16) & 0xff), uint8_t( (x>>24) & 0xff ) };
            frame(arr,4);
        }

        void ostream:: put(const uint64_t &x)
        {
            static const uint64_t mask = YACK_U64(0xff);
            const uint8_t arr[8] =
            {
                uint8_t(x&mask),       uint8_t((x>>8) &mask), uint8_t((x>>16)&mask), uint8_t((x>>24)&mask),
                uint8_t((x>>32)&mask), uint8_t((x>>40)&mask), uint8_t((x>>48)&mask), uint8_t((x>>56)&mask)
            };
            frame(arr,8);
        }

    }

}
