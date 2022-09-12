
#include "yack/math/fitting/variables.hpp"
#include "yack/math/fitting/replica.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

namespace
{

}


YACK_UTEST(fitting_vars)
{
    fitting::primary::handle    v = new fitting::primary("hello",1);
    fitting::variable::pointer  w = new fitting::replica("world",v);
    std::cerr << v << std::endl;
    std::cerr << w << std::endl;

    YACK_SIZEOF(fitting::variables);

    fitting::shared_vars shared = new fitting::variables();
    fitting::variables  &vars   = *shared;

    vars("hello",3);
    vars("world",1);

    vars << "a:b";
    std::cerr << "vars=" << vars << std::endl;

}
YACK_UDONE()
