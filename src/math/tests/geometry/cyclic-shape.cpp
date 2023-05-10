#include "yack/math/geometry/cyclic-shape.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace math;

namespace
{
    typedef v2d<double> vertex;
}

YACK_UTEST(cyclic_shape)
{
    YACK_SIZEOF( node2D<float>       );
    YACK_SIZEOF( node2D<double>      );
    YACK_SIZEOF( node2D<long double> );

    cyclic_contour<double> shape;
    shape << vertex(1,0) << vertex(0,1) << vertex(-0.7,0.7);

    shape.update();
    shape.save("shape.dat");
}
YACK_UDONE()

