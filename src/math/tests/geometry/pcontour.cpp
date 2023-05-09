
#include "yack/math/geometry/pcontour.hpp"
#include "yack/utest/run.hpp"
#include "yack/math/triplet.hpp"
#include "yack/string.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;
using namespace math;

typedef v2d<double>      vertex;



YACK_UTEST(pcontour)
{
    YACK_SIZEOF(periodic_contour<double>);


    size_t N = 4;

    if(argc>1)
    {
        N = ios::ascii::convert::to<size_t>(argv[1],"N");
    }

    periodic_contour<double> shape(N);

    for(size_t i=0;i<N;++i)
    {
        const double th = (i*numeric<double>::two_pi) / N;
        shape[i+1] = vertex( 1.0*cos(th), 1.0 * sin(th) );
    }


    shape.update();

    if(false)
    {
        ios::ocstream fp( vformat("metrics%lu.dat",N) );
        for(size_t i=1;i<=N;++i)
        {
            fp("%g %g %g %g %g %g %g %g\n",
               shape.theta[i],
               shape.curv[i],
               shape.speed[i].x,
               shape.speed[i].y,
               shape.accel[i].x,
               shape.accel[i].y);
        }
    }

    //shape.save_speed("speed.dat");
    //shape.save_curv("curv.dat");

    const size_t np = 1000;
    shape.save("shape.dat");
    shape.save_shape("spline.dat",np);

    std::cerr << "cmax=" << shape.cmax << std::endl;

    if(false)
        for(size_t i=1;i<=shape.size();++i)
        {
            const double th = shape.theta[i];

            std::cerr << "  theta = " << std::setw(15) << th;
            std::cerr << "  curv  = " << std::setw(15) << shape.curv[i];
            std::cerr << " | speed = " << shape.speed[i];
            std::cerr << " | " << shape.spd(th);
            std::cerr << std::endl;
        }

    return 0;


    for(unsigned iter=1;iter<=10;++iter)
    {
        for(size_t k=0;k<10;++k)
        {
            for(size_t i=shape.size();i>0;--i)
            {
                shape[i] += 0.01*(1.0-shape.curv[i]) * shape.nvec[i];
            }
            shape.update();
            std::cerr << "curv=" << shape.curv << std::endl;
        }

        shape.save( "shape.dat", true );
        shape.save_shape( "spline.dat", np, true);

    }


}
YACK_UDONE()

