

#include "yack/apex/natural.hpp"
#include "yack/arith/base2.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/endian.hpp"
#include "yack/randomized/rand.hpp"

using namespace yack;

YACK_UTEST(apex_ari)
{

    randomized::rand_  ran;

    std::cerr << "[FACTORIAL]" << std::endl;
    for(size_t i=0;i<=32;++i)
    {
        const apn f = apn::factorial(i);
        std::cerr << i << "! = " <<  f << " (#bits=" << f.bits() << ")"<< std::endl;
    }

    std::cerr << "[GCD]" << std::endl;
    for(size_t i=1;i<=1024;++i)
    {
        const apn I = i;
        for(size_t j=1;j<=1024;++j)
        {
            const apn J = i;
            const apn g = apn::gcd(I,J);
            YACK_ASSERT(0==I%g);
            YACK_ASSERT(0==J%g);
        }
    }
}
YACK_UDONE()

