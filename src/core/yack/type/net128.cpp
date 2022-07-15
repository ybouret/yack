
#include "yack/type/net128.hpp"
#include <cstring>
#include <iostream>

namespace yack
{

    net128_t:: net128_t() throw() :  h()
    {
        memset(h,0,sizeof(h));
    }

    net128_t:: ~net128_t() throw()
    {
        memset(h,0,sizeof(h));
    }

    net128_t:: net128_t(const net128_t &other) throw() : h()
    {
        memcpy(h,other.h,sizeof(h));
    }

    net128_t & net128_t:: operator=(const net128_t &other) throw()
    {
        memmove(h,other.h,sizeof(h));
        return *this;
    }

    uint8_t & net128_t:: operator[](const size_t i) throw()
    {
        assert(i<sizeof(h));
        return h[i];
    }

    const uint8_t & net128_t:: operator[](const size_t i) const throw()
    {
        assert(i<sizeof(h));
        return h[i];
    }

    net128_t:: net128_t(const uint8_t b)  throw() : h()
    {
        memset(h,b,sizeof(h));
    }


    std::ostream & operator<<(std::ostream &os, const net128_t &U)
    {
        os << unsigned(U[0]);
        for(size_t i=1;i<16;++i)
        {
            os << ':' << unsigned(U[i]);
        }
        return os;
    }

}
