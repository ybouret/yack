#include "yack/apex/primes.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(aprimes)
{
    apex::primes & P = apex::primes::instance();
    std::cerr << P.call_sign << std::endl;

    YACK_SIZEOF(apn);
    
}
YACK_UDONE()

