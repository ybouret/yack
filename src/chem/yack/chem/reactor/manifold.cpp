
#include "yack/chem/reactor.hpp"

namespace yack
{
    
    namespace chemical
    {

        void reactor:: make_manifold()
        {
            static const char fn[] = "[reactor.manifold] ";

            YACK_CHEM_PRINTLN(fn << "building all detached...");

            const group   __empty;
            vector<group> detached(L,__empty); assert(L==detached.size());

            for(const group *source = related.head; source; source=source->next)
            {
                for(size_t i=L;i>0;--i) detached[i].release();
                for(const gnode *lhs=source->head;lhs;lhs=lhs->next)
                {
                    const equilibrium &LHS = **lhs;
                    const size_t       LID = *LHS;
                    group             &LGP = detached[LID];

                    for(const gnode *rhs = lhs->next;rhs;rhs=rhs->next)
                    {
                        const equilibrium &RHS = **rhs;
                        const size_t       RID = *RHS;

                        if( LHS.detached_of(RHS) )
                        {
                            LGP           << &RHS;
                            detached[RID] << &LHS;
                        }
                    }
                }
                lattice(std::cerr,"",detached);
            }
            



            

        }

    }

}


