

#include "yack/chem/eqs/groups.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace chemical
    {

        groups:: ~groups() throw()                                {}
        groups::  groups() throw()        : cxx_list_of<group>()  {}
        groups::  groups(const groups &G) : cxx_list_of<group>(G) {}


        std::ostream & operator<<(std::ostream &os, const groups &G)
        {
            os << '{';
            const group *g = G.head;
            if(g)
            {
                os << std::endl << '\t' << *g;
                for(g=g->next;g;g=g->next)
                {
                    os << ',' << std::endl << '\t' << *g;
                }
                os << std::endl;
            }
            os << '}';
            return os;
        }

        static inline int compare_groups(const group *lhs, const group *rhs) throw()
        {
            assert(lhs);assert(rhs);
            const size_t L = lhs->size;
            const size_t R = rhs->size;
            if(L<R)
            {
                return -1;
            }
            else
            {
                if(R<L)
                {
                    return 1;
                }
                else
                {
                    const gnode *lnode = lhs->head;
                    const gnode *rnode = rhs->head;
                    for(size_t i=L;i>0;--i)
                    {
                        assert(lnode);
                        assert(rnode);
                        const size_t LI = ***lnode;
                        const size_t RI = ***rnode;
                        if(LI<RI)
                        {
                            return -1;
                        }
                        else
                        {
                            if(RI<LI)
                            {
                                return 1;
                            }
                            else
                            {
                                lnode=lnode->next;
                                rnode=rnode->next;
                            }
                        }
                    }
                    return 0;
                }
            }
        }

        void groups:: sort()
        {
            for(group *g=head;g;g=g->next)
            {
                g->sort();
            }
            merge_list_of<group>::sort(*this,compare_groups);
        }
        
        bool groups:: includes(const equilibrium &eq) const throw()
        {
            for(const group *g=head;g;g=g->next)
            {
                if(g->includes(eq)) return true;
            }
            return false;
        }

        bool groups:: contains(const group &sub) const throw()
        {
            for(const group *g=head;g;g=g->next)
            {
                if(g->contains(sub)) return true;
            }
            return false;
        }


        const group * groups:: find_first(const equilibrium &eq) const throw()
        {
            for(const group *g=head;g;g=g->next)
            {
                if(g->includes(eq)) return g;
            }
            return NULL;
        }

        const group * groups:: find_next(const group *g, const equilibrium &eq) const throw()
        {
            assert(g);
            for(g=g->next;g;g=g->next)
            {
                if(g->includes(eq)) return g;
            }
            return NULL;
        }

    }

}
