

#include "yack/chem/eqs/group.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace chemical
    {

        group:: ~group() throw()
        {
        }

        group:: group() throw() : object(), group_type(), next(0), prev(0) {}

        
        bool group:: accepts(const equilibrium &rhs) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                const equilibrium &lhs = **node;
                if( lhs.attached_to(rhs) ) return true;
            }
            return false;
        }

        bool group:: includes(const equilibrium &rhs) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                const equilibrium &lhs = **node;
                if( &lhs == &rhs ) return true;
            }
            return false;
        }
        
        bool group:: excludes(const equilibrium &rhs) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                const equilibrium &lhs = **node;
                if( &lhs == &rhs ) return false;
            }
            return true;
        }
        
        bool group:: contains(const group &sub) const throw()
        {
            for(const gnode *node=sub.head;node;node=node->next)
            {
                if( excludes( **node) ) return false;
            }
            return true;
        }

        
        group:: group(const group &other) : object(), group_type(other), next(0), prev(0)
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


        static inline int compare_gnodes(const gnode *lhs, const gnode *rhs) throw()
        {
            return comparison::increasing(***lhs,***rhs);
        }

        void group:: sort()
        {
            merge_list_of<gnode>::sort(*this,compare_gnodes);
        }

        bool group:: is_ortho() const throw()
        {
            for(const gnode *lhs=head;lhs;lhs=lhs->next)
            {
                const equilibrium &LHS = **lhs;
                for(const gnode *rhs=lhs->next;rhs;rhs=rhs->next)
                {
                    const equilibrium &RHS = **rhs;
                    if(!LHS.detached_of(RHS)) return false;
                }
            }
            return true;
        }


    }

}


