#include "yack/chem/mix/groups.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/counting/comb.hpp"

namespace yack
{
    namespace chemical
    {

        static inline bool is_acceptable( const readable<equilibrium *> &party,
                                         const matrix<bool>            &detached)
        {

#if 0
            std::cerr << "testing {" << host.name << "}";
            for(size_t i=1;i<=party.size();++i)
            {
                std::cerr << "+{" << party[i]->name << "}";
            }
            std::cerr << std::endl;
#endif

            const size_t k = party.size();
            for(size_t i=k;i>1;--i)
            {
                const size_t          lhs = **party[i];
                const readable<bool> &det = detached[lhs];
                for(size_t j=i-1;j>0;--j)
                {
                    const size_t rhs = **party[j];
                    if(!det[rhs])
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        groups:: groups(const group &attached, const matrix<bool> &detached) : group::list()
        {
            assert(attached.size>0);

            std::cerr << "Building Detached from " << attached << std::endl;

            vector<equilibrium *> guest(attached.size-1,as_capacity);
            vector<equilibrium *> party(attached.size-1,as_capacity);

            for(const gnode *xnode=attached.head;xnode;xnode=xnode->next)
            {
                const equilibrium    &ex = **xnode;
                const size_t          ix = *ex;
                const readable<bool> &dx = detached[ix];

                // building possible guests...
                guest.free();
                for(const gnode *ynode=xnode->next;ynode;ynode=ynode->next)
                {
                    const equilibrium &ey = **ynode;
                    const size_t       iy = *ey;
                    if(dx[iy])
                    {
                        guest << (equilibrium *) &ey;
                    }
                }


                const size_t n = guest.size();
                if(n<=0)
                {
                    // no possible co-evaluation
                    push_back( group::new_from(ex) );
                }
                else
                {
                    // testing all co-evaluations
                    for(size_t k=n;k>0;--k)
                    {
                        combination comb(n,k);
                        do {
                            comb.extract(party,guest); assert(k==party.size());
                            if(is_acceptable(party,detached))
                            {
                                group * g = push_back( group::new_from(ex) );
                                for(size_t i=1;i<=k;++i)
                                {
                                    *g << & *party[i];
                                }
                                assert(g->is_valid());
                                assert(g->is_ortho());
                            }
                        }
                        while( comb.next() );
                    }
                }
            }

            sort();
            
            // reduction
            {
                groups temp;
                while(size)
                {
                    group *lhs = pop_front();
                    bool   inc = false;
                    for(const group *rhs=head;rhs;rhs=rhs->next)
                    {
                        if(rhs->size>lhs->size && rhs->contains(*lhs) )
                        {
                            inc = true;
                            break;
                        }
                    }
                    if(inc)
                    {
                        delete lhs;
                    }
                    else
                    {
                        temp.push_back(lhs);
                    }
                }
                swap_with(temp);

            }
            std::cerr << *this << std::endl;


        }

    }

}
