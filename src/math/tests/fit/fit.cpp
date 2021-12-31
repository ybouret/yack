#include "yack/math/fit/variables.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(fit)
{

    fit::variables vars, v1, v2;
    vars << "t0" << "D1" << "D2";
    std::cerr << vars << std::endl;

    v1 << vars["t0"];
    v1("D", vars["D1"] );
    std::cerr << "v1=" << v1 << std::endl;


    v2 << "t0";
    v2("D", vars["D2"] );
    std::cerr << "v2=" << v2 << std::endl;


}
YACK_UDONE()

