
#include "yack/math/geometry/pcontour.hpp"
#include "yack/utest/run.hpp"
#include "yack/math/triplet.hpp"
#include "yack/string.hpp"

using namespace yack;
using namespace math;

YACK_UTEST(pcontour)
{
    YACK_SIZEOF(periodic_contour<double>);

    const size_t N = 4;

    periodic_contour<double> shape(N);
    typedef v2d<double>      vertex;

    for(size_t i=0;i<N;++i)
    {
        const double th = (i*numeric<double>::two_pi) / N;
        shape[i+1] = vertex( cos(th), 0.8 * sin(th) );
    }


    shape.update();

    shape.save_speed("speed.dat");
    shape.save_curv("curv.dat");

    const size_t np = 1000;
    shape.save("shape0.dat");
    shape.save_accel("accel0.dat");
    shape.save_shape("spline0.dat",np);

    for(unsigned iter=1;iter<=5;++iter)
    {
        for(size_t k=0;k<10;++k)
        {
            for(size_t i=shape.size();i>0;--i)
            {
                shape[i] += 0.1 * shape.nvec[i];
            }
            shape.update();
        }

        shape.save_shape( vformat("spline%u.dat",iter), np);
        shape.save( vformat("shape%u.dat",iter) );
        shape.save_accel( vformat("accel%u.dat",iter));

    }


}
YACK_UDONE()

