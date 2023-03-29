
#include "yack/chem/realm/reactor.hpp"
#include "yack/chem/eqs/aftermath.hpp"

namespace yack
{
    namespace chemical
    {

        reactor:: ~reactor() noexcept
        {
        }

        reactor:: reactor(const domain &_) :
        spot_object(),
        dom(_),
        xlim(),
        xadd(),
        xmul(),
        Corg(dom.M),
        blocked(dom.N),
        running(dom.N),
        Xi(dom.L),
        Cs(dom.L,dom.M),
        next(0),
        prev(0)
        {
        }

        void reactor:: operator()(writable<double> &C0, const readable<double> &K)
        {

            // load Corg into sub_level description

            dom.shrink(Corg,C0);

            dom.spdisp(std::cerr,Corg) << std::endl;
            
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
