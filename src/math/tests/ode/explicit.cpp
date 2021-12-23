#include "yack/math/ode/explicit/cash-karp.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(explicit)
{

    ode::cash_karp<float> rkck;
    rkck.make(2);
    
}
YACK_UDONE()
