


#include "yack/chem/reactor/equilibrium/tier.hpp"

namespace yack {

    namespace chemical {
        
        eq_tier:: ~eq_tier() throw() {}


        eq_tier:: eq_tier() :
        bounded( new eq_group() ),
        roaming( new eq_group() ),
        special( new eq_group() )
        {}

    }

}

