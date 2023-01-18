
#include "yack/chem/reactor/species/tier.hpp"

namespace yack {

    namespace chemical {

        sp_tier:: sp_tier() : conserved( new sp_group() ), unbounded( new sp_group() ) {}

        sp_tier:: ~sp_tier() throw() {}


        void sp_tier:: display(const xmlog &xml, const char * const pfx) const
        {
            const size_t n = conserved->size + unbounded->size;
            
            YACK_XMLOG(xml, "-- using " << n << " species --");
            if(conserved->size) { YACK_XMLOG(xml, "-- " << pfx << ".conserved = " << conserved); }
            if(unbounded->size) { YACK_XMLOG(xml, "-- " << pfx << ".unbounded = " << unbounded); }
        }

    }

}

