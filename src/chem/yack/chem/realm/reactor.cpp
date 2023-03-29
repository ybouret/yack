
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
        blocked(dom.L),
        running(dom.L),
        Xi(dom.L),
        Cs(dom.L,dom.M),
        next(0),
        prev(0)
        {
        }

        void reactor:: solve(const xmlog            &xml,
                             writable<double>       &C0,
                             const readable<double> &K)
        {

            YACK_XMLSUB(xml, "reactor");
            // load Corg into sub_level description
            dom.shrink(Corg,C0);

            if(xml.verbose)
            {
                dom.spdisp(*xml << "Corg = ",sub_level,Corg) << std::endl;
                dom.eqdisp(*xml << "K    = ",top_level,K)    << std::endl;
            }


            // determining all aftermaths
            size_t active = 0;
            for(const eq_node *en=dom.head;en;en=en->next)
            {
                const equilibrium &  eq = ***en;
                const size_t         gi = eq.indx[top_level];
                const size_t         i  = eq.indx[sub_level];
                writable<double>   & Ci = Cs[i];
                const double         Ki = K[gi];
                const aftermath      am = aftermath::solve(sub_level,eq,Ki,Corg,Ci,xlim,xmul,xadd);

                switch( am.state )
                {
                    case is_blocked:
                        blocked[i] = true;
                        running[i] = false;
                        Xi[i]      = 0;
                        break;

                    case is_running:
                        ++active;
                        blocked[i] = false;
                        running[i] = true;
                        Xi[i]      = am.value;
                        break;
                }

                if(xml.verbose)
                {
                    dom.eqfmt.pad( *xml << eq, eq) << ": " << am << std::endl;
                }
            }

            YACK_XMLOG(xml,"#active = " << active);

        }

    }

}
