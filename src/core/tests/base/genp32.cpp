#include "yack/arith/prime.hpp"
#include "yack/string.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace {

    static inline void check16()
    {
        uint64_t prev = core::primes::p8[0];
        YACK_ASSERT(prime64::is(prev));
        for(size_t i=1;i<core::primes::pi16;++i)
        {
            const uint64_t curr = core::primes::get16(i);
            YACK_ASSERT(curr>prev);
            YACK_ASSERT(prime64::is(curr));
            prev = curr;
        }
    }
}

YACK_UTEST(genp32)
{
    check16();

    if(argc>1)
    {
        char            buffer[256];
        const string    fn = argv[1];
        ios::characters line;

        ios::icstream fp(fn);
        size_t        count = 0;
        while(fp.gets(line))
        {
            memset(buffer,0,sizeof(buffer));
            if( line.fill(buffer,sizeof(buffer)-1) )
            {
                //std::cerr << "[" << buffer << "]" << std::endl;
                ++count;
            }
        }
        std::cerr << "count=" << count << std::endl;
    }



}
YACK_UDONE()

