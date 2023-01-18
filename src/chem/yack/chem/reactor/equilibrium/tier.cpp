


#include "yack/chem/reactor/equilibrium/tier.hpp"

namespace yack {

    namespace chemical {
        
        eq_tier:: ~eq_tier() throw() {}


        eq_tier:: eq_tier() :
        delimited( new eq_group() ),
        reac_only( new eq_group() ),
        prod_only( new eq_group() ),
        undefined( new eq_group() )
        {}


    }

}

