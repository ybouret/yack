

#include "yack/chem/eqs/tier.hpp"

namespace yack
{
    namespace chemical
    {
        eq_tier::  eq_tier() throw() : roaming(), bounded(), unbridled(), regulated() {}
        eq_tier:: ~eq_tier() throw() {}

        static inline void update(sp_book       &tribe,
                                  const eq_team &source)
        {
            for(const eq_node *node=source.head;node;node=node->next)
                (**node).update(tribe);
        }


        static inline void compose(sp_list       &tgt,
                                   const eq_team &src,
                                   const library &lib)
        {
            tgt.release();
            sp_book tribe;
            update(tribe,src);
            tgt.load(tribe,lib);
        }

        static inline void compose(sp_list       &tgt,
                                   const eq_team &lhs,
                                   const eq_team &rhs,
                                   const library &lib)
        {
            tgt.release();
            sp_book tribe;
            update(tribe,lhs);
            update(tribe,rhs);
            tgt.load(tribe,lib);
        }

        void eq_tier:: compile_with(const library &lib)
        {
            compose( coerce(committed), roaming, bounded, lib);
            compose( coerce(unbridled), roaming, lib);
            compose( coerce(regulated), bounded, lib);
            coerce(regulated).shed(unbridled);
        }
    }

}
