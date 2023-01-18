


#include "yack/chem/reactor/equilibrium/tier.hpp"

namespace yack {

    namespace chemical {
        
        eq_tier:: ~eq_tier() throw() {}


        eq_tier:: eq_tier() :
        delimited( new eq_group() ),
        reac_only( new eq_group() ),
        prod_only( new eq_group() ) 
        {}

        bool eq_tier:: dispatch(const gnode *gn)
        {
            assert(gn);
            const equilibrium &eq = gn->host;
            if(eq.reac->size<=0)
            {
                assert(eq.prod->size>0);
                coerce(*prod_only) << *gn;
                return true;
            }
            else
            {
                if(eq.prod->size<=0)
                {
                    coerce(*reac_only) << *gn;
                    return true;
                }
                else
                {
                    coerce(*delimited) << *gn;
                    return false;
                }
            }
        }

    }

}

