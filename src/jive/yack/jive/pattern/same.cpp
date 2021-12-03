
#include "yack/jive/pattern/all.hpp"
#include "yack/type/fourcc.h"


namespace yack
{
    namespace jive
    {

        template <typename JOKER> static inline
        bool are_same_joker(const pattern &lhs, const pattern &rhs) throw()
        {
            return pattern::are_same( **lhs.as<JOKER>(), **rhs.as<JOKER>() );
        }

        template <typename LOGIC> static inline
        bool are_same_logic(const pattern &lhs, const pattern &rhs) throw()
        {
            const logical *L = lhs.as<LOGIC>();
            const logical *R = rhs.as<LOGIC>();

            if(L->size!=R->size)
            {
                return false;
            }
            else
            {
                for(const motif *l=L->head,*r=R->head;l;l=l->next,r=r->next)
                {
                    if( !pattern::are_same(**l,**r) ) return false;
                }
                return true;
            }
        }

#define YACK_JIVE_PATTERN_SAME_BASIC(CLASS) \
case CLASS::mark: return lhs.as<CLASS>()->is_same_than( rhs.as<CLASS>() )

#define YACK_JIVE_PATTERN_SAME_JOKER(CLASS) \
case CLASS::mark: return are_same_joker<CLASS>(lhs,rhs)

#define YACK_JIVE_PATTERN_SAME_LOGIC(CLASS) \
case CLASS::mark: return are_same_logic<CLASS>(lhs,rhs)


        bool pattern:: are_same(const pattern &lhs, const pattern &rhs) throw()
        {
            if(lhs.uuid!=rhs.uuid)
            {
                return false;
            }
            else
            {
                switch(lhs.uuid)
                {
                        YACK_JIVE_PATTERN_SAME_BASIC(single);
                        YACK_JIVE_PATTERN_SAME_BASIC(within);
                        YACK_JIVE_PATTERN_SAME_BASIC(except);

                        YACK_JIVE_PATTERN_SAME_JOKER(optional);
                        YACK_JIVE_PATTERN_SAME_JOKER(at_least);
                        YACK_JIVE_PATTERN_SAME_JOKER(counting);

                        YACK_JIVE_PATTERN_SAME_LOGIC(op_and);
                        YACK_JIVE_PATTERN_SAME_LOGIC(op_or);
                        YACK_JIVE_PATTERN_SAME_LOGIC(op_none);


                    default:
                        break;
                }
                return false;
            }
        }

    }

}
