#include "yack/hashing/hmac.hpp"
#include "yack/hashing/md5.hpp"
#include "yack/utest/run.hpp"

#include <cstring>

using namespace yack;

namespace
{
    static inline
    void test_hmac(const digest &key,
                   const char   *data,
                   const char   *hres)
    {
        assert(data);
        assert(hres);
        hashing::md5 H;
        const size_t data_len = strlen(data);
        const digest expected = hres;
        std::cerr << "key       = " << key << std::endl;
        std::cerr << "key_len   = " << key.size() << std::endl;
        std::cerr << "data      = " << data << std::endl;
        std::cerr << "data_len  = " << data_len << std::endl;
        std::cerr << "expected  = " << expected << std::endl;

        hashing::hmac HMAC(H,*key,key.size());
        
        HMAC.set(H);
        H(data,data_len);
        const digest signature = HMAC.get(H);
        std::cerr << "signature = " << signature << std::endl;

        if(signature!=expected) throw exception("invalid RFC2401 test");
    }
}

YACK_UTEST(hashing_hmac)
{


    {
        const digest key(16,0x0b);
        test_hmac(key,"Hi There","9294727a3638bb1c13f48ef8158bfc9d");
    }

    {
        const digest key("Jefe",4);
        test_hmac(key,"what do ya want for nothing?","750c783e6ab0b503eaa86e310a5db738");
    }

    {
        const digest key(16,0xAA);
        char         data[64];
        memset(data,0,sizeof(data));
        memset(data,0xDD,50);
        test_hmac(key,data,"56be34521d144c88dbb8c733f0e8b3f6");
    }
}
YACK_UDONE()


