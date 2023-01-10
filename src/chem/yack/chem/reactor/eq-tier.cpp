

#include "yack/chem/reactor/eq-tier.hpp"

namespace yack {

    namespace chemical {


        eq_group::  eq_group() throw() : object(), eq_group_() {}
        eq_group:: ~eq_group() throw() {}


        std::ostream & operator<<(std::ostream &os, const eq_group &self)
        {
            os << "{ ";
            const eq_gnode *node=self.head;
            if(node)
            {
                os << (***node).host.name;
                for(node=node->next;node;node=node->next)
                {
                    os << ", " << (***node).host.name;
                }
            }
            return os << " }";
        }

    }

}


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

