#include "yack/arith/prime.hpp"
#include "yack/string.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/ios/ascii/convert.hpp"

#include "yack/utest/run.hpp"
#include <cstring>

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

    uint64_t prev = core::primes::h16;
    uint64_t smax = 0;

    if(argc>1)
    {
        char            buffer[256];
        const string    fn = argv[1];
        ios::characters line;
        size_t          every = 0;
        ios::icstream    fp(fn);
        ios::ocstream    zp("shift.dat");
        size_t           count = 0;
        while(fp.gets(line))
        {
            memset(buffer,0,sizeof(buffer));
            if( line.fill(buffer,sizeof(buffer)-1) )
            {
                //std::cerr << "[" << buffer << "]" << std::endl;
                ++count;
                const uint64_t curr = ios::ascii::convert::to<uint64_t>(buffer,"prime number");
                YACK_ASSERT(curr>prev);
                const uint64_t diff = curr-prev; YACK_ASSERT(0==(diff&1));
                const uint64_t half = diff>>1;   YACK_ASSERT(half>0);
                const uint64_t shft = half-1;
                if(shft>smax)
                {
                    smax = shft;
                }
                if(shft>255) throw exception("shift is too high!!");
                zp.write( uint8_t(shft) );
                prev = curr;
                if(++every>=100000)
                {
                    (std::cerr << '.').flush();
                    every=0;
                }
            }
        }
        std::cerr << std::endl;
        std::cerr << "count=" << count << std::endl;
        std::cerr << "smax =" << smax  << std::endl;
    }



}
YACK_UDONE()

