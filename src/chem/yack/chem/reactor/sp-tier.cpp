
#include "yack/chem/reactor/sp-tier.hpp"

namespace yack {

    namespace chemical {

        sp_tier:: sp_tier() : conserved( new sp_group() ), unbounded( new sp_group() ) {}

        sp_tier:: ~sp_tier() throw() {}
        
    }

}

