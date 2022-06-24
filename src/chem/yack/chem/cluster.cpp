#include "yack/chem/cluster.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace chemical
    {
        cluster::  cluster() throw() : object(), vlist(), next(0), prev(0) {}

        cluster:: ~cluster() throw() {}

        cluster::  cluster(const cluster &other) :
        object(),
        vlist(other), next(0), prev(0)
        {}

        std::ostream & operator<<(std::ostream &os, const cluster &cls)
        {
            os << '{';
            const vnode *node = cls.head;
            if(node) {
                os << (**node).name;
                for(node=node->next;node;node=node->next)
                    os << ',' << (**node).name;
            }
            os << '}';
            return os;
        }

        static inline int compare_vnodes(const vnode *lhs, const vnode *rhs) throw()
        {
            assert(lhs);
            assert(rhs);
            const equilibrium &L = **lhs, &R = **rhs;  assert(L.name!=R.name);
            const size_t       l = *L,     r = *R;     assert(l!=r);

            return l<r ? -1 : 1;
        }


        bool cluster:: isValid() const throw()
        {
            for(const vnode *node=head;node;node=node->next)
            {
                if(node->next)
                {
                    const equilibrium &lhs = **node;
                    const equilibrium &rhs = **(node->next);
                    if( *lhs >= *rhs) return false;
                }
            }
            return true;
        }

        bool cluster:: isOrtho() const throw()
        {
            for(const vnode *node=head;node;node=node->next)
            {
                if(node->next)
                {
                    const equilibrium &lhs = **node;
                    const equilibrium &rhs = **(node->next);
                    if( lhs.attached(rhs) ) return false;
                }
            }
            return true;
        }

        void cluster:: update() throw()
        {
            merge_list_of<vnode>::sort(*this,compare_vnodes);
        }

        bool cluster:: matches(const cluster &rhs) const throw()
        {
            const cluster &lhs = *this;
            size_t         i   = lhs.size;
            if( i == rhs.size )
            {
                const vnode *L = lhs.head;
                const vnode *R = rhs.head;
                for(;i>0;--i,L=L->next,R=R->next)
                {
                    const size_t l = ***L;
                    const size_t r = ***R;
                    if( l != r) return false;
                }

                return true;
            }
            else
            {
                return false;
            }
        }

        bool cluster:: carries(const equilibrium &eq) const throw()
        {
            const equilibrium *target = &eq;
            for(const vnode *node=head;node;node=node->next)
            {
                const equilibrium *source = & **node;
                if( target == source ) return true;
            }
            return false;
        }

        bool cluster:: accepts(const equilibrium &target) const throw()
        {
            assert( !carries(target) );
            for(const vnode *node=head;node;node=node->next)
            {
                const equilibrium &source = **node;
                if(source.attached(target)) return true;
            }
            return false;
        }

        bool cluster:: includes(const cluster &other) const throw()
        {
            for(const vnode *node=other.head;node;node=node->next)
            {
                if( ! carries( **node) ) return false;
            }
            return true;
        }

        int cluster:: compare(const cluster *lhs, const cluster *rhs) throw()
        {
            assert(lhs);
            assert(rhs);
            const size_t lsize = lhs->size;
            const size_t rsize = rhs->size;

            // check sizes
            switch(__sign::of(lsize,rsize))
            {
                case negative: assert(lsize<rsize); return -1;
                case positive: assert(rsize<lsize); return  1;
                case __zero__:
                    break;
            }


            // lexicographic order for same sizes
            size_t i = lsize;
            for(const vnode *L=lhs->head, *R=rhs->head;i>0;--i,L=L->next,R=R->next)
            {
                const size_t il = ***L;
                const size_t ir = ***R;
                switch(__sign::of(il,ir))
                {
                    case negative: assert(il<ir); return -1;
                    case positive: assert(ir<il); return  1;
                    case __zero__: break;
                }
            }

            // shouldn't happen here since all clusters are different
            return 0;
        }

    }

}
