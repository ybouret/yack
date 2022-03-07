

#include "yack/arith/raw-zero-flux.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(zflux)
{
    for(unit_t width=1;width<=10;++width)
    {
        const raw_zero_flux zf(width);
        for(unit_t x=-1000;x<=1000;++x)
        {
            const unit_t i=zf(x);
            YACK_ASSERT(i>=0);
            YACK_ASSERT(i<width);

        }
    }
}
YACK_UDONE()

