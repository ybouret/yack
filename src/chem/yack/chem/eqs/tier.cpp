

#include "yack/chem/eqs/tier.hpp"

namespace yack
{
    namespace chemical
    {
        eq_tier::  eq_tier() throw() : roaming(), bounded(), unbridled(), regulated() {}
        eq_tier:: ~eq_tier() throw() {}

        void eq_tier:: compose(sp_list &target, const eq_team &source)
        {
            target.release();
            addrbook tribe;
            for(const eq_node *node=source.head;node;node=node->next)
                (**node).update(tribe);
            target.load(tribe);
        }

        void eq_tier:: compile()
        {
            compose( coerce(unbridled), roaming );
            compose( coerce(regulated), bounded );
            coerce(regulated).shed(unbridled);
        }
    }

}
