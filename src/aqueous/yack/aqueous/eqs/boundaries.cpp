
#include "yack/aqueous/eqs/boundaries.hpp"

namespace yack
{
    namespace aqueous
    {
        
        boundaries:: ~boundaries() noexcept
        {

        }

        boundaries:: boundaries(const zl_proxy &zlp,
                                const sp_proxy &spp) noexcept :
        zlimits(zlp),
        spore(spp)
        {
        }

        void  boundaries:: expand(const double   xi,
                                  const species &sp)
        {
            (void) annex(spore,xi,sp);
        }

    }
}
