#include "yack/color/rgba.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(color_rgb)
{
    randomized::rand_ ran;

    YACK_SIZEOF(color::rgb);
    YACK_SIZEOF(color::rgba);

    color::rgb c;
    for(size_t iter=0;iter<10;++iter)
    {
        for(size_t n=c.size();n>0;--n)
        {
            (c[n] = ran.to<uint8_t>());
        }
    }

    if(false)
    {
        for(size_t i=0;i<256;++i)
        {
            std::cout << " YURT(" << std::setw(3) << i << ")";
            std::cout << ",";
            if(0 == (1+i)%16) std::cout << std::endl;
        }
    }

}
YACK_UDONE()

