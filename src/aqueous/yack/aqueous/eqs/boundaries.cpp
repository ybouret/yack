
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

        void  boundaries:: insert(const double   xi,
                                  const species &sp)
        {
            assert(xi<0);
            
            zl_node *node =  annex(spore,xi,sp);
        }

    }
}
