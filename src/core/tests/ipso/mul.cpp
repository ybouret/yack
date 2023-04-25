#include "yack/ipso/mul.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"


using namespace yack;


YACK_UTEST(ipso_mul)
{


    ipso::mul<float> dfm;
    dfm << -0.01 << 30 << 0.2;
    std::cerr << "dfm=" << dfm << std::endl;
    std::cerr << dfm.reduce() << std::endl;

    ipso::mul<apq> dqm;
    dqm << apq(-1,100) << apq(30) << apq(2,10);
    std::cerr << "dqm=" << dqm << std::endl;
    std::cerr << dqm.reduce() << std::endl;

    ipso::mul< complex<float>  > dcfm;
    

}
YACK_UDONE()

