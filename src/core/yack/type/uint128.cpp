
#include "yack/type/uint128.hpp"
#include "yack/type/hexa.hpp"
#include <cstring>
#include <iostream>

namespace yack
{

    uint128_t:: uint128_t() noexcept :  h()
    {
        memset(h,0,sizeof(h));
    }

    uint128_t:: uint128_t(uint64_t lo) noexcept : h()
    {
        memset(h+bytes/2,0,bytes/2);
        for(size_t i=0;i<sizeof(uint64_t);++i)
        {
            h[i] = static_cast<uint8_t>(lo);
            lo >>= 8;
        }
    }

    uint128_t:: uint128_t(const uint8_t data[]) noexcept : h()
    {
        memcpy(h,data,bytes);
    }



    uint128_t:: ~uint128_t() noexcept
    {
        memset(h,0,sizeof(h));
    }

    uint128_t:: uint128_t(const uint128_t &other) noexcept : h()
    {
        memcpy(h,other.h,sizeof(h));
    }

    uint128_t & uint128_t:: operator=(const uint128_t &other) noexcept
    {
        memmove(h,other.h,sizeof(h));
        return *this;
    }

    uint8_t & uint128_t:: operator[](const size_t i) noexcept
    {
        assert(i<sizeof(h));
        return h[i];
    }

    const uint8_t & uint128_t:: operator[](const size_t i) const noexcept
    {
        assert(i<sizeof(h));
        return h[i];
    }

    


    std::ostream & operator<<(std::ostream &os, const uint128_t &U)
    {
        for(int i=int(uint128_t::bytes)-1;i>=0;--i)
        {
            os << hexa::lowercase_text[U[i]];
        }
        return os;
    }

    bool  operator==(const uint128_t &lhs, const uint128_t &rhs) noexcept
    {
        return 0 == memcmp(lhs.h,rhs.h,uint128_t::bytes);
    }

    bool  operator!=(const uint128_t &lhs, const uint128_t &rhs) noexcept
    {
        return 0 != memcmp(lhs.h,rhs.h,uint128_t::bytes);
    }



}
