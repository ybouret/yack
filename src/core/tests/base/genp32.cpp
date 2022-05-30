#include "yack/arith/prime.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(genp32)
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
YACK_UDONE()

