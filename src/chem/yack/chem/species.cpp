#include "yack/chem/species.hpp"
#include "yack/ios/ostream.hpp"
#include <iomanip>

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

        std::ostream & species:: frame(std::ostream &os, const readable<double> &C) const
        {
            assert(C.size()>=**this);
            os << std::setw(15) << C[ **this ];
            return os;
        }

        ios::ostream & species:: frame(ios::ostream &os, const readable<double> &C) const
        {
            assert(C.size()>=**this);
            os("%.15g",C[ **this ]);
            return os;
        }
        
    }

}
