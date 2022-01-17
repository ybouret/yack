#include "yack/field/in4d.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace  yack;

namespace
{
    template <typename FIELD>
    static inline void display(FIELD &F)
    {
        std::cerr << "F.dims=" << F.dims << "/" << F.dimensions << "/"<< F.space << "/" << F.coords << std::endl;
        std::cerr << "F.name=" << F.name << " is " << F << std::endl;
        std::cerr << std::endl;
    }
}


YACK_UTEST(fields)
{
    randomized::rand_ ran;

    field1D<string> s( "s", layout1D(-5,6) );
    display(s);

    for(unit_t i=s.lower;i<=s.upper;++i)
    {
        s[i] = bring::get<string>(ran);
    }




    field2D<string> s2( "s2", layout2D( coord2D(-2,-3), coord2D(4,5) ));
    display(s2);

    for(unit_t y=s2.lower.y;y<=s2.upper.y;++y)
    {
        for(unit_t x=s2.lower.x;x<=s2.upper.x;++x)
        {
            s2[y][x] = bring::get<string>(ran);
        }
    }


    field3D<string> s3( "s3", layout3D( coord3D(-1,-2,-3), coord3D(4,5,6) ));
    display(s3);

    for(unit_t z=s3.lower.z;z<=s3.upper.z;++z)
    {
        for(unit_t y=s3.lower.y;y<=s3.upper.y;++y)
        {
            for(unit_t x=s3.lower.x;x<=s3.upper.x;++x)
            {
                s3[z][y][x] = bring::get<string>(ran);
            }
        }
    }

    field4D<string> s4( "s4", layout4D( coord4D(-1,-2,-3,-4), coord4D(4,5,6,7) ));
    display(s4);

    for(unit_t w=s4.lower.w;w<=s4.upper.w;++w)
    {
        for(unit_t z=s4.lower.z;z<=s4.upper.z;++z)
        {
            for(unit_t y=s4.lower.y;y<=s4.upper.y;++y)
            {
                for(unit_t x=s4.lower.x;x<=s4.upper.x;++x)
                {
                    s4[w][z][y][x] = bring::get<string>(ran);
                }
            }
        }
    }

}
YACK_UDONE()
