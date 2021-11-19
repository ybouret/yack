#include "yack/hashing/to.hpp"
#include "yack/hashing/md.hpp"
#include "yack/hashing/md5.hpp"
#include "yack/kr/digest.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/fmt/hexa.hpp"

using  namespace yack;

YACK_UTEST(hashing_key)
{
    
    hashing::to_key<uint32_t,hashing::md5> kh;
    {
        const digest d = hashing::md::of(kh, "a", 1);
        std::cerr << d << std::endl;
    }

    {
        const uint32_t k   = kh("a");
        std::cerr << "k=" << ios::hexa(k) << std::endl;
    }

    {
        const digest   d("a",1);
        const uint32_t k = kh(d);
        std::cerr << "k=" << ios::hexa(k) << std::endl;
    }

    {
        const char a = 'a';
        const uint32_t k = kh(a);
        std::cerr << "k=" << ios::hexa(k) << std::endl;
    }
    
}
YACK_UDONE()

