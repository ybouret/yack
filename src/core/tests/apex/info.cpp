#include "yack/apex.hpp"
#include "yack/apex/primes.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(apex_info)
{
    YACK_SIZEOF(apn);
    YACK_SIZEOF(apz);
    YACK_SIZEOF(apq);
    YACK_SIZEOF(apex::prime_knot);
    

}
YACK_UDONE()
