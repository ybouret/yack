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
        w(zflux)
        {
        }


        const uint8_t * bitrow:: addr() const throw()
        {
            assert(p);
            return static_cast<const uint8_t*>(p);
        }

        uint8_t * bitrow:: addr()   throw()
        {
            assert(p);
            return static_cast<uint8_t*>(p);
        }



    }
}
