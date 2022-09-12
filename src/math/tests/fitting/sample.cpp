
#include "yack/math/fitting/sample-of.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;


YACK_UTEST(fitting_sample)
{

    const size_t   N = 10;
    vector<double> X(N);
    vector<double> Y(N);
    vector<double> Z(N);

    fitting::sample_of<double,double> s1("s1",X,Y,Z);
    fitting::variables               &vars = *s1;
    vars << "a:b";

    std::cerr << s1.name << " #" << s1.dimension() << std::endl;


}
YACK_UDONE()

