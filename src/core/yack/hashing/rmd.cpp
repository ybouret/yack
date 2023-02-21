#include "yack/hashing/rmd.hpp"
#include <cstring>
namespace yack
{
    
    namespace hashing
    {
        rmd:: ~rmd() noexcept
        {
            reset();
            memset(X,0,sizeof(X));
            memset(B,0,sizeof(B));
        }
        
#if defined(_MSC_VER)
        // init B
#pragma warning ( disable : 4351 )
#endif
        rmd:: rmd() noexcept :
        length(0),
        nx(0),
        nb(0),
        X(),
        B()
        {
            memset(X,0,sizeof(X));
            memset(B,0,sizeof(B));
        }
        
        
        void rmd:: reset() noexcept
        {
            coerce(length) = nx = nb = 0;

        }
        
#define B2X() ( ( uint32_t(B[3]) << 24 ) |  ( uint32_t(B[2]) << 16 ) |  ( uint32_t(B[1]) << 8 ) | uint32_t(B[0]) )
        bool rmd:: store( uint8_t b ) noexcept
        {
            assert(nb<4);
            assert(nx<16);
            ++coerce(length);
            B[nb++] = b;
            if(nb>=4)
            {
                X[nx++] = B2X();
                nb = 0;
                if( nx >= 16 )
                {
                    nx = 0;
                    return true;
                }
            }
            return false;
        }
        
        const uint32_t * rmd:: block() const noexcept
        {
            assert(0==nx);
            assert(0==nb);
            assert(length>0);
            return X;
        }
        
        const uint8_t * rmd:: flush() noexcept
        {
            assert(nx<16);
            assert(nb<4);
            for(size_t i=nb;i<4;++i)
                B[i] = 0;
            X[nx] = B2X();
            return (uint8_t *)X;
        }
        
        uint32_t rmd:: lswlen() const noexcept
        {
            return uint32_t(length);
        }
        
        uint32_t rmd:: mswlen() const noexcept
        {
            uint64_t tmp = length;
            tmp >>= 32;
            tmp &=  0xFFFFFFFFUL;
            return uint32_t(tmp);
        }
        
    }
}
