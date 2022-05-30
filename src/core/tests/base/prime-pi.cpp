#include "yack/arith/prime-pi.hpp"
#include "yack/arith/prime.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    static inline size_t manual_count(const unsigned n)
    {
        size_t         count = 0;
        const uint64_t pmax  = uint64_t(1) << n;
        uint64_t p = 0;
        while( (p = prime64::next(++p)) <= pmax )
        {
            //std::cerr << p << std::endl;
            ++count;
        }
        return count;
    }
}

YACK_UTEST(primePi)
{
    std::cerr << "#prime_pi=" << core::prime_pi::count << std::endl;
    for(size_t i=0;i<core::prime_pi::count;++i)
    {
        std::cerr << "PrimePi(" << std::setw(2) << i << ") = " << core::prime_pi::table[i] << std::endl;
    }

    {
        const uint64_t pi8  = core::primes::n8;
        YACK_CHECK(pi8 ==core::prime_pi::table[8]);
    }

    {
        const uint64_t pi16 = core::primes::n8+core::primes::n16;
        YACK_CHECK(pi16==core::prime_pi::table[16]);
    }

    if(argc>1)
    {
        const unsigned n = ios::ascii::convert::to<unsigned>(argv[1],"n");
        if(n>=core::prime_pi::count)
        {
            throw exception("%u exceed prime_pi table size",n);
        }
        //std::cerr << "count(" << n << ")=" << manual_count(n) << std::endl;
        for(size_t i=0;i<=n;++i)
        {
            const size_t   count = manual_count(i);
            const uint64_t table = core::prime_pi::table[i];
            std::cerr << "@" << std::setw(3) << i << " : " << std::setw(6) << count << " / " << table << std::endl;
            YACK_ASSERT(count==table);
        }
    }

}
YACK_UDONE()


