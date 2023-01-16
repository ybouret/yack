
#include "yack/chem/reactor/balance/boundaries.hpp"

namespace yack {

    namespace chemical
    {

        boundaries:: boundaries(const size_t m) :
        limiting(m),
        amending(m)
        {
        }

        boundaries:: ~boundaries() throw()
        {
        }

        void boundaries:: probe(const readable<double> &C,
                                const actors           &A,
                                const sp_fund          &F)
        {
            limiting.free();
            amending.free();
            for(const actor *a=A->head;a;a=a->next)
            {
                const species &s = **a;
                const size_t   j = *s;
                const double   c = C[j];
                if(c>=0)
                {
                    // that's a limiting concentration
                    limiting(c/a->nu,s,F);
                }
                else
                {
                    // that's an amending concentration
                    amending(c/a->nu,s,F);
                }
            }
        }

    }

}

