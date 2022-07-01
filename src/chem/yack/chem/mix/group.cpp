
#include "yack/chem/mix/group.hpp"
#include "yack/exception.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace chemical
    {

        group:: ~group() throw()
        {
        }

        group:: group() throw()
        {

        }

        int group:: compare_nodes(const gnode *lhs,
                                  const gnode *rhs)  
        {
            const equilibrium &L = **lhs;
            const equilibrium &R = **rhs;
            const size_t       l = *L;
            const size_t       r = *R;
            if(l<r)
            {
                return -1;
            }
            else
            {
                if(r<l)
                    return 1;
                else
                {
                    throw exception("in chemical::group: same index for '%s' and '%s'", L.name(), R.name() );
                    return 0;
                }
            }

        }

        void group:: sort()
        {
            merge_list_of<gnode>::sort(*this,compare_nodes);
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

        bool group:: contains(const equilibrium &eq) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                if( & **node == &eq ) return true;
            }
            return false;
        }


        bool group:: contains(const group &g) const throw()
        {
            for(const gnode *guest = g.head; guest; guest=guest->next)
            {
                if( ! contains(**guest) ) return false;
            }
            return true;
        }

        bool group:: attached(const equilibrium &eq) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                if( eq.attached(**node) ) return true;
            }
            return false;
        }

        bool group:: detached(const equilibrium &eq) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                if( !eq.detached(**node)) return false;
            }
            return true;
        }

    }

}
