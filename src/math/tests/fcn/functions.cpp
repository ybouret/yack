#include "yack/math/fcn/qerf.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ocstream.hpp"

using namespace yack;
using namespace math;


YACK_UTEST(functions)
{
    {
        ios::ocstream fc("qerfc.dat");
        ios::ocstream ff("qerf.dat");

        for(float x=-10; x<=10;x += 0.01f)
        {
            const double xx = double(x);
            ff("%g %g %g %g\n", xx, double( qerf<float>(x)  ), qerf<double>(xx),  double(qerf<long double>(xx))  );
            fc("%g %g %g %g\n", xx, double( qerfc<float>(x) ), qerfc<double>(xx), double(qerfc<long double>(xx)) );
        }
    }
}
YACK_UDONE()

