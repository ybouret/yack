#include "yack/math/ode/driver.hpp"
#include "yack/math/ode/explicit/rk45/cash-karp.hpp"
#include "yack/math/ode/explicit/rk45.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(explicit)
{

    ode::rk45<float> step( new ode::cash_karp<float>() );
    
}
YACK_UDONE()
