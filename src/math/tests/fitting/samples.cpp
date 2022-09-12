#include "yack/math/fitting/samples-of.hpp"
#include "yack/type/v2d.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

typedef v2d<double> vtx;

YACK_UTEST(fitting_samples)
{
    const size_t    N1 = 10;
    vector<vtx>     P1(N1);
    vector<double>  Z1(N1);
    vector<double>  F1(N1);

    const size_t    N2 = 15;
    vector<vtx>     P2(N2);
    vector<double>  Z2(N2);
    vector<double>  F2(N2);

    fitting::samples_of<vtx,double> circles("circles");
    fitting::sample_of<vtx,double> &circle1 = circles("circle1",P1,Z1,F1);
    fitting::sample_of<vtx,double> &circle2 = circles("circle2",P2,Z2,F2);

    std::cerr << circles.name << " #" << circles.dimension() << std::endl;
    std::cerr << circle1.name << " #" << circle1.dimension() << std::endl;
    std::cerr << circle2.name << " #" << circle2.dimension() << std::endl;


}
YACK_UDONE()

