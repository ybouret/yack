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
        shape << vertex( 1.1 * cos(th), 0.9 * sin(th) );
    }


    shape.update();
    shape.center();

    std::cerr << "nodes: " << shape.size << std::endl;
    std::cerr << "sides: " << shape.sides.size << std::endl;
    std::cerr << shape << std::endl;
    std::cerr << shape.sides << std::endl;

    shape.save("shape.dat");
    shape.save_t("shape_t.dat");
    shape.save_n("shape_n.dat");
    shape.save_a("shape_a.dat");
    shape.smooth("smooth.dat",16);

    return 0;

    ios::ocstream fp("kappa.dat");

    for(unsigned iter=1;iter<=10;++iter)
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
        shape.save_n( vformat("shape%u-n.dat",iter) );
        shape.save_kappa( vformat("shape%u-kappa.dat",iter) );

    }


}
YACK_UDONE()

