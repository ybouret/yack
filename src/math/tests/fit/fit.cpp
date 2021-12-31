#include "yack/math/fit/variables.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(fit)
{

    fit::variables vars;
    vars << "t0" << "D1" << "D2";

    std::cerr << vars << std::endl;
}
YACK_UDONE()

