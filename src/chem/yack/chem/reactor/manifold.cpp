
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

            for(const enode *lhs = lattice.head(); lhs; lhs=lhs->next)
            {
                const equilibrium &LHS = ***lhs;
                const size_t       LID = *LHS;
                group             &LGP = detached[LID];

                for(const enode *rhs = lhs->next;rhs;rhs=rhs->next)
                {
                    const equilibrium &RHS = ***rhs;
                    const size_t       RID = *RHS;

                    if( LHS.detached_of(RHS) )
                    {
                        LGP           << &RHS;
                        detached[RID] << &LHS;
                    }
                }
            }

            lattice(std::cerr,"",detached);
            //std::cerr << detached << std::endl;


            

        }

    }

}


