#include "yack/gfx/bitrow.hpp"

namespace yack
{
    namespace graphic
    {
        
        bitrow:: ~bitrow() throw()
        {}

        bitrow:: bitrow(void            *entry,
                        const zero_flux &zflux) throw() :
        p(entry),
        z(zflux)
        {
        }

    }
}
