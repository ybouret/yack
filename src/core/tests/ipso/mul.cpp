#include "yack/ipso/mul.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"


using namespace yack;


YACK_UTEST(ipso_mul)
{


    ipso::mul<float> dfm;
    dfm << -0.01f << 30.0f << 0.2f;
    std::cerr << "dfm=" << dfm << std::endl;
    std::cerr << dfm.reduce() << std::endl;

    ipso::mul<apq> dqm;
    dqm << apq(-1,100) << apq(30) << apq(2,10);
    std::cerr << "dqm=" << dqm << std::endl;
    std::cerr << dqm.reduce() << std::endl;

    ipso::mul< complex<float> > dcfm;
    dcfm << -0.01f << 30.0f<< 0.2f;
    std::cerr << "dcfm=" << dcfm << std::endl;
    std::cerr << dcfm.reduce() << std::endl;
}
YACK_UDONE()

