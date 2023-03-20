//! \file

#ifndef YACK_AQUEOUS_COLL_CACHES_INCLUDED
#define YACK_AQUEOUS_COLL_CACHES_INCLUDED 1

#include "yack/aqueous/eqs/boundaries.hpp"

namespace yack
{
    namespace aqueous
    {

        namespace collecting
        {
            
            class caches
            {
            public:
                explicit caches();
                virtual ~caches() noexcept;

                const zl_proxy zlp;
                const sp_proxy spp;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(caches);
            };
        }

    }

}

#endif
