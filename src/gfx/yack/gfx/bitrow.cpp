#include "yack/gfx/bitrow.hpp"

namespace yack
{
    namespace graphic
    {
        
        bitrow:: ~bitrow() noexcept
        {}

        bitrow:: bitrow(void            *entry,
                        const zero_flux &zflux) noexcept :
        p(entry),
        w(zflux)
        {
        }


        const uint8_t * bitrow:: addr() const noexcept
        {
            assert(p);
            return static_cast<const uint8_t*>(p);
        }

        uint8_t * bitrow:: addr()   noexcept
        {
            assert(p);
            return static_cast<uint8_t*>(p);
        }



    }
}
