
#include "yack/apex/integer.hpp"
#include "yack/utest/run.hpp"
#include "yack/randomized/rand.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;

YACK_UTEST(apz)
{
    apz m = -2;
    apz z =  0;
    apz p =  5;
    
    {
        ios::ocstream fp("apz.dat");
        std::cerr << m << " ->" << m.serialize(fp) << std::endl;
        std::cerr << z << " ->" << z.serialize(fp) << std::endl;
        std::cerr << p << " ->" << p.serialize(fp) << std::endl;

    }
}
YACK_UDONE()
