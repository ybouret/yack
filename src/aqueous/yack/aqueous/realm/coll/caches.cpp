#include "yack/aqueous/realm/coll/caches.hpp"

namespace yack
{
    namespace aqueous
    {

        namespace collecting
        {

            caches:: ~caches() noexcept
            {
            }

            caches:: caches() :
            zlp( new zl_zpool() ),
            spp( new sp_zpool() ),
            eqp( new eq_zpool() )
            {
            }

        }

    }

}


