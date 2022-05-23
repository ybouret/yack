
#include "yack/utest/run.hpp"
#include "yack/field/in2d.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/counting/loop-on.hpp"
#include "yack/apex.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

typedef v2d<double> vertex;



YACK_UTEST(fmake)
{

    int nx=1;
    if(argc>1)
    {
        nx = ios::ascii::convert::to<int>(argv[1],"nx");
    }
    const int    ny=nx;


    const layout2D   dims(coord2D(-nx,-ny), coord2D(nx,ny));
    loop_on<coord2D> loop(dims.lower,dims.upper);

    vector<apq>    rho2;
    vector<double> dir;

    loop.boot();

    do
    {
        const coord2D iv = *loop;
        const vertex  v(iv.x,iv.y);
        std::cerr << "#" << loop.index << " : " << iv << std::endl;
        const apq r2 = iv.norm2();
        rho2 << r2;
        dir << v.angle();

    } while(loop.next());


    hsort(rho2,dir,comparison::increasing<apq>);
    for(size_t i=1;i<=rho2.size();++i)
    {
        std::cerr << "rho2=" << rho2[i] << " -> " << dir[i] << std::endl;
    }




}
YACK_UDONE()
