#include "yack/field/in4d.hpp"
#include "yack/utest/run.hpp"
#include "yack/hashing/sha1.hpp"
#include "yack/hashing/md.hpp"
#include "yack/kr/digest.hpp"

#include "../main.hpp"

using namespace  yack;

namespace
{
    template <typename FIELD>
    static inline void display(FIELD             &F,
                               hashing::function &H,
                               const digest      &D)
    {
        std::cerr << "F.dims=" << F.dims << "/" << F.dimensions << "/"<< F.space << "/" << F.coords << std::endl;
        std::cerr << "F.name=" << F.name << " is " << F << std::endl;

        typename FIELD::loop_type loop(F.lower,F.upper);
        std::cerr << "loop: " << loop.lower << " -> " << loop.upper << std::endl;

        H.set();
        loop.boot();
        do
        {
            H( F(loop) );
        }
        while(loop.next());
        const digest R = hashing::md::of(H);
        std::cerr << D << "=>" << R << std::endl;
        YACK_CHECK(D==R);
        std::cerr << std::endl;
    }

    template <typename FIELD>
    static inline void check_layout(const FIELD &F)
    {
        std::cerr << "testing layout" << F.dimensions << "D" << std::endl;
        typename FIELD::loop_type loop(F.lower,F.upper);
        loop.boot();
        do
        {
            const unit_t        idx = F.index_of( *loop );
            YACK_ASSERT(idx>=0);
            YACK_ASSERT(idx<unit_t(F.items));
            const cardinality_t crd = loop.index;
            YACK_ASSERT( crd-1 == cardinality_t(idx) );

            typename FIELD::coord_type pos = F.coord_of(idx);
            YACK_ASSERT(pos == *loop);
        }
        while(loop.next());
        std::cerr << std::endl;
    }

}


YACK_UTEST(fields)
{
    randomized::rand_ ran;
    hashing::sha1     H;

    field1D<string>   s( "s", layout1D(-5,6) );
    H.set();
    for(unit_t i=s.lower;i<=s.upper;++i)
    {
        H(s[i] = bring::get<string>(ran));
    }
    const digest md1 = hashing::md::of(H);
    display(s,H,md1);
    check_layout(s);
    s.print(std::cerr) << std::endl;

    

    field2D<string> s2( "s2", layout2D( coord2D(-2,-3), coord2D(4,5) ));
    H.set();
    for(unit_t y=s2.lower.y;y<=s2.upper.y;++y)
    {
        for(unit_t x=s2.lower.x;x<=s2.upper.x;++x)
        {
            H(s2[y][x] = bring::get<string>(ran));
        }
    }
    const digest md2 = hashing::md::of(H);
    display(s2,H,md2);
    check_layout(s2);


    field3D<string> s3( "s3", layout3D( coord3D(-1,-2,-3), coord3D(4,5,6) ));
    H.set();
    for(unit_t z=s3.lower.z;z<=s3.upper.z;++z)
    {
        for(unit_t y=s3.lower.y;y<=s3.upper.y;++y)
        {
            for(unit_t x=s3.lower.x;x<=s3.upper.x;++x)
            {
                H(s3[z][y][x] = bring::get<string>(ran));
            }
        }
    }
    const digest md3 = hashing::md::of(H);
    display(s3,H,md3);
    check_layout(s3);




    field4D<string> s4( "s4", layout4D( coord4D(-1,-2,-3,-4), coord4D(4,5,6,7) ));
    H.set();
    for(unit_t w=s4.lower.w;w<=s4.upper.w;++w)
    {
        for(unit_t z=s4.lower.z;z<=s4.upper.z;++z)
        {
            for(unit_t y=s4.lower.y;y<=s4.upper.y;++y)
            {
                for(unit_t x=s4.lower.x;x<=s4.upper.x;++x)
                {
                    H(s4[w][z][y][x] = bring::get<string>(ran));
                }
            }
        }
    }
    const digest md4 = hashing::md::of(H);
    display(s4,H,md4);
    check_layout(s4);


}
YACK_UDONE()
