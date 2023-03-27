#include "yack/aqueous/realm/collector.hpp"
#include <iomanip>

namespace yack
{

    namespace aqueous
    {
        void collector:: displace(writable<double> &C,
                                  const cluster    &W)
        {
            if(1==W.size)
            {
            }


            for(const sp_node *sn=W.spec.head;sn;sn=sn->next)
            {
                const species &s = ***sn;
                const size_t   j = s.indx[top_level];
                const double   c = C[j];
                xadd.free();
                xadd.push(c);
                for(const eq_node *en=W.head;en;en=en->next)
                {
                    const equilibrium      &E  = ***en;
                    const size_t            i  = E.indx[cat_level];
                    xadd.push(-c);
                    xadd.push(Cbal[i][j]);
                }
                C[j] = xadd.sum();
            }
        }
    }

}

