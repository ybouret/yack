
#include "yack/hashing/hmac.hpp"
#include <cstring>

namespace yack
{
    namespace hashing
    {
        hmac:: ~hmac() throw()
        {
            coerce(L) = 0;
            coerce(B) = 0;
        }

        hmac:: hmac(function       &H,
                    const void     *key_addr,
                    const size_t    key_size) :
        L(H.length),
        B(H.window),
        key(B),
        ikp(B),
        okp(B),
        tmp(L)
        {
            assert(!(NULL==key_addr&&key_size>0));
            setup(H,key_addr,key_size);
        }

        void hmac:: setup(function    &H,
                          const void  *key_addr,
                          const size_t key_size) throw()
        {
            assert(!(NULL==key_addr&&key_size>0));
            coerce(key).ldz();
            if(key_size>B)
            {
                H.set();
                H.run(key_addr,key_size);
                H.get(*coerce(key),L);
            }
            else
            {
                memcpy(*coerce(key), key_addr, key_size);
            }

            for(size_t i=B;i>0;--i)
            {
                coerce(ikp[i]) = uint8_t(key[i] ^ 0x36);
                coerce(okp[i]) = uint8_t(key[i] ^ 0x5c);
            }

        }

        void hmac:: set(function &H) throw()
        {
            H.set();
            H(ikp);
        }

        const memory::ro_buffer & hmac:: get(function &H) throw()
        {
            H.get(*tmp,L);
            H.set();
            H(okp);
            H(tmp);
            H.get(*tmp,L);
            return tmp;
        }

    }

}

