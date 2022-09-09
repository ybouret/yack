#include "yack/apex/natural.hpp"
#include "yack/utest/run.hpp"


using namespace yack;

YACK_UTEST(aprsa)
{



    const apn p  = 97;
    const apn q  = 83;
    const apn n  = p*q;
    const apn pm = p-1;
    const apn qm = q-1;
    const apn l  = apn::lcm(pm,qm);
    const apn e  = 17;
    const apn d  = apn::mod_inv(e,l);

    std::cerr << "p=" << p << std::endl;
    std::cerr << "q=" << q << std::endl;
    std::cerr << "n=" << n << std::endl;
    std::cerr << "l=" << l << std::endl;
    std::cerr << "e=" << e << std::endl;
    std::cerr << "d=" << d << std::endl;

    for(apn m=0;m<n;++m)
    {
        const apn c = apn::mod_exp(m,e,n);
        const apn q = apn::mod_exp(c,d,n);
        std::cerr << m << " -> " << c << " -> " << q << std::endl;
        if(m!=q) throw exception("mismatch!");
    }


}
YACK_UDONE()

