#include "yack/math/geometry/cyclic-shape.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"
#include "yack/ios/ascii/convert.hpp"

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

    size_t np = 3;
    if(argc>1)
    {
        np = ios::ascii::convert::to<size_t>(argv[1],"np");
    }

    for(size_t i=0;i<np;++i)
    {
        const double th = (i * numeric<double>::two_pi)/np;
        shape << vertex( 1.5 * cos(th), 0.8 * sin(th) );
    }


    shape.update();
    shape.center();

    std::cerr << "nodes: " << shape.size << std::endl;
    std::cerr << "sides: " << shape.sides.size << std::endl;


    shape.save("shape0.dat");
    shape.smooth("smooth0.dat",16);


    ios::ocstream fp("kappa.dat");

    for(unsigned iter=1;iter<=5;++iter)
    {
        for(size_t k=0;k<50;++k)
        {
            node2D<double> *node = shape.head;
            for(size_t i=shape.size;i>0;--i,node=node->next)
            {
                //node->r += 0.01*(1.0-hypothenuse(node->r)) * node->n;
                node->r += 0.01*(1.0-node->kappa)*node->n;
            }
            shape.update();
            //shape.center();

        }
        {
            const node2D<double> *node = shape.head;
            for(size_t i=1;i<=shape.size;++i,node=node->next)
            {
                fp("%lu %g\n",i,node->kappa);
            }
            fp << '\n';
        }
        shape.save(   vformat("shape%u.dat",iter) );
        shape.smooth( vformat("smooth%u.dat",iter), 16 );

    }


}
YACK_UDONE()

