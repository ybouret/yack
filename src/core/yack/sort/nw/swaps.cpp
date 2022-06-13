#include "swaps.hpp"

namespace yack
{
    namespace nwsrt
    {
        swaps_:: ~swaps_() throw()
        {
        }

        swaps_:: swaps_(const char  *sid,
                        const size_t dim) throw() :
        name(sid),
        size(dim)
        {
        }
        

    }
}
