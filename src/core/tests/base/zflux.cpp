

#include "yack/arith/zero-flux.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(zflux)
{

    zero_flux_using<core::zero_flux::raw_proc_for> toto(1);
    
    for(unit_t width=1;width<=10;++width)
    {
        const core::zero_flux zf(width);
        const core::zero_flux::proc raw = core::zero_flux::raw_proc_for(zf);
        const core::zero_flux::proc cxx = core::zero_flux::cxx_proc_for(zf);

        for(unit_t x=-1000;x<=1000;++x)
        {
            {
                const unit_t i=raw(zf,x);
                YACK_ASSERT(i>=0);
                YACK_ASSERT(i<width);
            }

            {
                const unit_t i=cxx(zf,x);
                YACK_ASSERT(i>0);
                YACK_ASSERT(i<=width);
            }
        }
    }
}
YACK_UDONE()

