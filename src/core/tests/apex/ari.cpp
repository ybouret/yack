

#include "yack/apex/natural.hpp"
#include "yack/arith/base2.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/endian.hpp"
#include "yack/randomized/rand.hpp"

using namespace yack;

YACK_UTEST(apex_ari)
{

    randomized::rand_  ran;
    for(size_t i=0;i<=32;++i)
    {
        const apn f = apn::factorial(i);
        std::cerr << i << "! = " <<  f << " (#bits=" << f.bits() << ")"<< std::endl;
    }

}
YACK_UDONE()

