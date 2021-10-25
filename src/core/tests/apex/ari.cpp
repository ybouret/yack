

#include "yack/apex/natural.hpp"
#include "yack/arith/base2.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/endian.hpp"
#include "yack/randomized/rand.hpp"
#include "yack/arith/ipower.hpp"

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


    std::cerr << "[MOD_INV]" << std::endl;
    {
        const apn n = 7919;
        for(apn b=1;b<n;++b)
        {
            const apn I = apn::mod_inv(b,n);
            const apn p = (I*b)%n;
            YACK_ASSERT(1==p);
        }
    }
    
    std::cerr << "[MOD_EXP]" << std::endl;
    {
        const apn n = 104729;
        for(size_t iter=0;iter<10;++iter)
        {
            const apn b(ran,n.bits()-1);
            const apn e(ran,2+ran.leq(6));
            std::cerr << '(' << b << '^' << e << ')' << '[' << n << ']' << std::endl;
            const apn p = apn::mod_exp(b,e,n);
            const apn q = ipower(b,e)%n;
            YACK_ASSERT(p==q);
        }
    }




}
YACK_UDONE()

