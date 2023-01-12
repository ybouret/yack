

#include "yack/chem/reactor/equilibrium/group.hpp"

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
