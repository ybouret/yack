
#include "yack/math/geometry/pcontour.hpp"
#include "yack/utest/run.hpp"
#include "yack/math/triplet.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(pcontour)
{
    YACK_SIZEOF(periodic_contour<double>);
    periodic_contour<double> shape(3);

    shape[1] = v2d<double>(1,0);
    shape[2] = v2d<double>(0,1.5);
    shape[3] = v2d<double>(-2,-0.1);

    shape.update();

    shape.save("shape.dat");
    shape.save_accel("accel.dat");
    shape.save_speed("speed.dat");

}
YACK_UDONE()

