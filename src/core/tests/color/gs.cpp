
#include "yack/color/rgba.hpp"
#include "yack/utest/run.hpp"
#include "yack/color/unit-real.hpp"

using namespace yack;

YACK_UTEST(color_gs)
{

    static const unsigned n =3*255+1;
    static const unsigned m = n-1;
    {
        std::cout << "{\n";

        for(unsigned i=0;i<n;++i)
        {
            std::cout << "(" << i << ".0f/" << m << ")";
            if(i<m)
            {
                std::cout << ",";
            }
            if(0==((i+1)%8))
            {
                std::cout << "\n";
            }
        }
        std::cout << "};\n";
    }

    {
        std::cout << "{\n";

        for(unsigned i=0;i<n;++i)
        {
            const float    f = float(i)/m;
            const uint8_t  u = color::unit_real<float>::to_byte(f);
            std::cout << unsigned(u);
            if(i<m)
            {
                std::cout << ",";
            }
            if(0==((i+1)%8))
            {
                std::cout << "\n";
            }
        }
        std::cout << "};\n";
    }


}
YACK_UDONE()
