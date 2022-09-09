#include "yack/utest/run.hpp"
#include "yack/arith/prime.hpp"

using namespace yack;

YACK_UTEST(prime_stab)
{
    const size_t np = core::primes::pi16;
    for(size_t i=0;i<np;++i)
    {
        const size_t p = core::primes::get16(i);
        std::cerr << p << std::endl;
    }
}
YACK_UDONE()

