
#include "yack/math/fitting/variables.hpp"
#include "yack/math/fitting/variable/replica.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

namespace
{

}


YACK_UTEST(fitting_vars)
{
    {
        fitting::primary::handle    v = new fitting::primary("hello",1);
        fitting::variable::pointer  w = new fitting::replica("world",v);
        std::cerr << v << std::endl;
        std::cerr << w << std::endl;

        YACK_SIZEOF(fitting::variables);

        fitting::variables  vars;

        vars.use("hello",3);
        vars.use("world",1);

        vars << "a:b";
        std::cerr << "vars=" << vars << std::endl;
    }

    {
        fitting::variables vars,v1,v2;

        vars << "t0:D1:D2";

        std::cerr << "vars=" << vars << std::endl;

        v1("t0",vars);
        v2("t0",vars);

        v1("D",vars,"D1");
        v2("D",vars,"D2");


        std::cerr << "v1=" << v1 << std::endl;
        std::cerr << "v2=" << v2 << std::endl;

        {
            fitting::variables a;
            a = vars;
            std::cerr << "\t" << a << std::endl;
            a = v1;
            std::cerr << "\t" << a << std::endl;
            a = v2;
            std::cerr << "\t" << a << std::endl;
        }

    }



}
YACK_UDONE()
