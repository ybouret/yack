


#include "yack/chem/reactor/equilibrium/tier.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/string/plural.hpp"

namespace yack {

    namespace chemical {
        
        eq_tier:: ~eq_tier() noexcept {}


        eq_tier:: eq_tier() :
        delimited( new eq_group() ),
        reac_only( new eq_group() ),
        prod_only( new eq_group() ),
        undefined( new eq_group() ),
        balancing( new eq_repo()  )
        {}


        void eq_tier:: display(const xmlog &xml, const char * const pfx) const
        {
            const size_t n =  reac_only->size + prod_only->size + undefined->size + delimited->size;

            YACK_XMLOG(xml, "-- using " << n << " equilibri" << plural::a(n) << " --");

            if(reac_only->size) {
                YACK_XMLOG(xml, "-- " << pfx << ".reac_only = " << reac_only);
            }

            if(prod_only->size) {
                YACK_XMLOG(xml, "-- " << pfx << ".prod_only = " << prod_only);
            }

            if(undefined->size) {
                YACK_XMLOG(xml, "-- " << pfx << ".undefined = " << undefined);
            }

            if(delimited->size) {
                YACK_XMLOG(xml, "-- " << pfx << ".delimited = " << delimited);
            }
            
            if(balancing->size) {
                YACK_XMLOG(xml, "-- " << pfx << ".balancing = " << balancing);
            }

        }

    }

}

