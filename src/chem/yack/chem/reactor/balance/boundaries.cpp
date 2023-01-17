
#include "yack/chem/reactor/balance/boundaries.hpp"

namespace yack {

    namespace chemical
    {

        boundaries:: boundaries(const size_t m, const sp_fund &f) :
        limiting(f),
        amending(m)
        {
        }

        boundaries:: ~boundaries() throw()
        {
        }

        bool boundaries:: probe(const readable<double>  &C,
                                const actors            &A,
                                const sp_fund           &F,
                                const readable<bool>    &B)
        {
            limiting.free();
            amending.free();
            for(const actor *a=A->head;a;a=a->next)
            {
                const species &s = **a;
                const size_t   j = *s;    if(!B[j]) continue;
                const double   c = C[j];
                if(c>=0)
                {
                    // that's a limiting concentration
                    limiting(c/a->nu,s);
                }
                else
                {
                    // that's an amending concentration
                    amending((-c)/a->nu,s,F);
                }
            }

            return amending.size()>0;
        }

    }

}

