#include "yack/aqueous/realm/collector.hpp"

namespace yack
{

    namespace aqueous
    {
        bool   collector:: needed_some_fixing(const actors &A, writable<double> &C)
        {
            assert(A.size>0);
            solo.clear();
            for(const actor *a=A.head;a;a=a->next)
            {
                const species &s = **a;
                const double   c = C[s.indx[top_level]];
                if(c<0)
                {
                    solo.insert( (-c)/a->nu, s);
                }
            }

            if(solo.size>0)
            {
                const zlimit &zl = **(solo.tail);
                const double  xi = zl.extent;
                for(const actor *a=A.head;a;a=a->next)
                {
                    const species &s = **a;
                    C[ s.indx[top_level] ] += (a->nu * xi);
                }
                zl.nullify(C);
                return true;
            }
            else
            {
                return false;
            }


        }

    }
}

