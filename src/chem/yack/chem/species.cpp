

#include "yack/chem/species.hpp"

namespace yack
{
    namespace chemical
    {

        species:: ~species() throw() {}

        species:: species(const species &sp) :
        entity(sp),
        z(sp.z),
        rank(sp.rank)
        {
        }

        
    }

}
