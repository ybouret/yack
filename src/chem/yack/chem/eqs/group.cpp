

#include "yack/chem/eqs/group.hpp"

namespace yack
{
    namespace chemical
    {

        group:: ~group() throw()
        {
        }

        group:: group() throw() : group_type(), next(0), prev(0) {}

        
        bool group:: connects(const equilibrium &rhs) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                const equilibrium &lhs = **node;
                if( lhs.connected_to(rhs) ) return true;
            }

            return false;
        }

        std::ostream & operator<<(std::ostream &os, const group &g)
        {
            os << '{';
            const gnode *node = g.head;
            if(node)
            {
                os << (**node).name;
                for(node=node->next;node;node=node->next)
                {
                    os << ',' << (**node).name;
                }
            }
            os << '}';
            return os;
        }

    }

}


