

#include "yack/chem/eqs/tier.hpp"

namespace yack
{
    namespace chemical
    {
        eq_tier::  eq_tier() throw() : roaming(), bounded(), unbridled(), regulated() {}
        eq_tier:: ~eq_tier() throw() {}


        static inline void update(addrbook &tribe, const eq_team &source)
        {
            for(const eq_node *node=source.head;node;node=node->next)
                (**node).update(tribe);
        }


        static inline void compose(sp_list       &tgt,
                                   const eq_team &src)
        {
            tgt.release();
            addrbook tribe;
            update(tribe,src);
            tgt.load(tribe);
        }

        static inline void compose(sp_list       &tgt,
                                   const eq_team &lhs,
                                   const eq_team &rhs)
        {
            tgt.release();
            addrbook tribe;
            update(tribe,lhs);
            update(tribe,rhs);
            tgt.load(tribe);
        }

        void eq_tier:: compile()
        {
            compose( coerce(committed), roaming, bounded);
            compose( coerce(unbridled), roaming );
            compose( coerce(regulated), bounded );
            coerce(regulated).shed(unbridled);
        }
    }

}
