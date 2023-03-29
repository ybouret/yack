
#include "yack/aqueous/realm/reactor.hpp"
#include "yack/aqueous/eqs/aftermath.hpp"

namespace yack
{
    namespace aqueous
    {

        reactor:: ~reactor() noexcept
        {
        }

        reactor:: reactor(const domain &_) :
        dom(_),
        Corg(dom.M),
        blocked(dom.N),
        running(dom.N),
        Xi(dom.L),
        Cs(dom.L,dom.M)
        {
        }

        void reactor:: operator()(writable<double> &C0, const readable<double> &K)
        {

            // load Corg
            dom.shrink(Corg,C0);

            //
            for(const eq_node *en=dom.head;en;en=en->next)
            {
                const equilibrium &  eq = ***en;
                const size_t         gi = eq.indx[top_level];
                const size_t         li = eq.indx[sub_level];
                writable<double>   & Ci = Cs[li];
                const aftermath      am = aftermath::solve(sub_level,eq, K[gi], Corg, Ci,xlim, xmul, xadd);
            }

        }

    }

}
