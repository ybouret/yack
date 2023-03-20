
#include "yack/aqueous/eqs/frontiers.hpp"

namespace yack
{
    namespace aqueous
    {

        frontiers:: ~frontiers() noexcept
        {
        }

        frontiers:: frontiers(const zl_proxy &zlp,
                              const sp_proxy &spp) noexcept :
        regular(spp),
        invalid(zlp,spp)
        {

        }

        void frontiers:: reopen() noexcept
        {
            regular.reset();
            invalid.clear();
        }

        void frontiers:: detect(const actors &A, const readable<double> &C)
        {
            
        }

    }

}

