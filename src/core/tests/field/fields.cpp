#include "yack/field/in3d.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace  yack;


YACK_UTEST(fields)
{
    randomized::rand_ ran;

    field1D<string> s( "s", layout1D(-5,6) );

    std::cerr << s.name << " is " << s << std::endl;

    for(unit_t i=s.lower;i<=s.upper;++i)
    {
        s[i] = bring::get<string>(ran);
    }

    for(unit_t i=s.lower;i<=s.upper;++i)
    {
        std::cerr << s[i] << std::endl;
    }


    field2D<string> s2( "s2", layout2D( coord2D(-2,-3), coord2D(4,5) ));
    std::cerr << s2.name << " is " << s2 << std::endl;

    for(unit_t y=s2.lower.y;y<=s2.upper.y;++y)
    {
        for(unit_t x=s2.lower.x;x<=s2.upper.x;++x)
        {
            s2[y][x] = bring::get<string>(ran);
        }
    }


    field3D<string> s3( "s3", layout3D( coord3D(-1,-2,-3), coord3D(4,5,6) ));
    std::cerr << s3.name << " is " << s3 << std::endl;



}
YACK_UDONE()
