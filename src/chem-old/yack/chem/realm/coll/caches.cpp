#include "yack/chem/realm/coll/caches.hpp"

namespace yack
{
    namespace chemical
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


