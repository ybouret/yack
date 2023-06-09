
#include "yack/chem/eqs/extents.hpp"

namespace yack
{
    namespace Chemical
    {

        Extents:: ~Extents() noexcept
        {
        }

        Extents:: Extents(const Species::Fund &speciesFund ) noexcept:
        reac(speciesFund),
        prod(speciesFund),
        kind(LimitedByNone)
        {
        }

        void Extents:: reset() noexcept
        {
            coerce(reac).clear();
            coerce(prod).clear();
            coerce(kind) = LimitedByNone;
        }



    }

}
