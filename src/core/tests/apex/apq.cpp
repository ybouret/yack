
#include "yack/apex/rational.hpp"
#include "yack/utest/run.hpp"
#include "yack/randomized/rand.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

YACK_UTEST(apq)
{
    randomized::rand_ ran;

    { apq q; std::cerr << q << std::endl; }
    { apq q = 7; std::cerr << q << std::endl; }
    { apq q(5,10); std::cerr << q << std::endl; }

    {
        
    }

}
YACK_UDONE()


