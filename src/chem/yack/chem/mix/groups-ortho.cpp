#include "yack/chem/mix/groups.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/counting/comb.hpp"

namespace yack
{
    namespace chemical
    {

        const groups::build_clusters_ groups::build_clusters = {};
        
        static inline bool is_acceptable(const readable<equilibrium *> &party,
                                         const matrix<bool>            &detached)
        {

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

        groups:: groups(const group        &attached,
                        const matrix<bool> &detached) : group::list()
        {
            assert(attached.size>0);


            //------------------------------------------------------------------
            //
            //
            // outer loop
            //
            //
            //------------------------------------------------------------------
            {
                vector<equilibrium *> guest(attached.size-1,as_capacity);
                vector<equilibrium *> party(attached.size-1,as_capacity);
                for(const gnode *xnode=attached.head;xnode;xnode=xnode->next)
                {
                    const equilibrium    &ex = **xnode;
                    const size_t          ix = *ex;
                    const readable<bool> &dx = detached[ix];

                    //----------------------------------------------------------
                    //
                    // building possible guests for ex
                    //
                    //----------------------------------------------------------
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
                        //------------------------------------------------------
                        // no possible co-evaluation
                        //------------------------------------------------------
                        push_back( group::start_from(ex) );
                    }
                    else
                    {
                        //------------------------------------------------------
                        // testing all co-evaluations
                        //------------------------------------------------------
                        for(size_t k=n;k>0;--k)
                        {
                            combination comb(n,k);
                            do {
                                comb.extract(party,guest); assert(k==party.size());
                                if(is_acceptable(party,detached))
                                {
                                    group * g = push_back( group::start_from(ex) );
                                    for(size_t i=1;i<=k;++i)
                                    {
                                        *g << & *party[i];
                                    }
                                    assert(g->is_valid()); // by construction
                                    assert(g->is_ortho()); // by construction
                                }
                            }
                            while( comb.next() );
                        }
                    }
                }
            }

            //------------------------------------------------------------------
            //
            //
            // full sort
            //
            //
            //------------------------------------------------------------------
            sort();

            //------------------------------------------------------------------
            //
            //
            // reduction
            //
            //
            //------------------------------------------------------------------
            {
                groups temp;
                while(size)
                {
                    group *lhs      = pop_front();
                    bool   included = false;
                    for(const group *rhs=head;rhs;rhs=rhs->next)
                    {
                        if(rhs->size>lhs->size && rhs->contains(*lhs) )
                        {
                            included = true;
                            break;
                        }
                    }

                    if(included)
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
        }

    }

}
