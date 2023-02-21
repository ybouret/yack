
#include "yack/hashing/hmac.hpp"
#include <cstring>

namespace yack
{
    namespace hashing
    {
        const char hmac:: prefix[] = "hmac-";

        hmac:: ~hmac() noexcept
        {
            coerce(L) = 0;
            coerce(B) = 0;
        }


#define YACK_HMAC_CTOR() \
L(H.length),\
B(H.window),\
key(B),\
ikp(B),\
okp(B),\
tmp(L)
        hmac:: hmac(function       &H,
                    const void     *key_addr,
                    const size_t    key_size) :
        YACK_HMAC_CTOR()
        {
            assert(!(NULL==key_addr&&key_size>0));
            setup(H,key_addr,key_size);
        }


        hmac:: hmac(function &H, const memory::ro_buffer &usr) :
        YACK_HMAC_CTOR()
        {
            setup(H,usr.ro_addr(),usr.measure());
        }

        void hmac:: setup(function    &H,
                          const void  *key_addr,
                          const size_t key_size) noexcept
        {
            assert(!(NULL==key_addr&&key_size>0));
            coerce(key).ldz();
            if(key_size>B)
            {
                H.set();
                H.run(key_addr,key_size);
                H.get(coerce(key)(),L);
            }
            else
            {
                memcpy(coerce(key)(), key_addr, key_size);
            }

            for(size_t i=B;i>0;--i)
            {
                coerce(ikp[i]) = uint8_t(key[i] ^ 0x36);
                coerce(okp[i]) = uint8_t(key[i] ^ 0x5c);
            }

        }

        void hmac:: set(function &H) noexcept
        {
            H.set();
            H(ikp);
        }

        const memory::ro_buffer & hmac:: get(function &H) noexcept
        {
            H.get(tmp(),L);
            H.set();
            H(okp);
            H(tmp);
            H.get(tmp(),L);
            return tmp;
        }

    }

}

