

#include "yack/chem/eqs/group.hpp"

namespace yack
{
    namespace chemical
    {

        group:: ~group() throw()
        {
        }

        group:: group() throw() : group_type(), next(0), prev(0) {}

        
        bool group:: accepts(const equilibrium &rhs) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                const equilibrium &lhs = **node;
                if( lhs.attached_to(rhs) ) return true;
            }
            return false;
        }

        group:: group(const group &other) : group_type(other)
        {
        }
        

        std::ostream & operator<<(std::ostream &os, const group &g)
        {
            os << '{';
            const gnode *node = g.head;
            if(node)
            {
                size_t count=1;
                os << (**node).name;
                for(node=node->next;node;node=node->next)
                {
                    os << ',';
                    if( 0 == (++count&7) ) os << std::endl << '\t';
                    os << (**node).name;

                }
            }
            os << '}';
            return os;
        }

    }

}


