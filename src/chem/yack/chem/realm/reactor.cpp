
#include "yack/chem/realm/reactor.hpp"
#include "yack/chem/eqs/aftermath.hpp"
#include <iomanip>
#include "yack/math/iota.hpp"

namespace yack
{
    using namespace math;

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
        sigma(dom.L),
        Cs(dom.L,dom.M),
        eqpxy( new eq_zpool() ),
        active(eqpxy),
        next(0),
        prev(0)
        {
        }


        struct eq_node_comparator
        {
            const readable<double> &Xi;

            inline int operator()(const eq_node *lhs,
                                  const eq_node *rhs) const noexcept
            {
                const size_t l = (***lhs).indx[sub_level];
                const size_t r = (***rhs).indx[sub_level];
                return comparison::increasing_abs(Xi[l],Xi[r]);
            }
        };

        void reactor:: solve(const xmlog            &xml,
                             writable<double>       &C0,
                             const readable<double> &K)
        {

            YACK_XMLSUB(xml, "reactor");
            const eq_node_comparator cmp = { Xi };

            // load Corg into sub_level description
            dom.shrink(Corg,C0);

            if(xml.verbose)
            {
                dom.spdisp(*xml << "Corg = ",sub_level,Corg) << std::endl;
                dom.eqdisp(*xml << "K    = ",top_level,K)    << std::endl;
            }


            // determining all aftermaths
            active.clear();
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
                        sigma[i]   = 0;
                        break;

                    case is_running:
                        blocked[i] = false;
                        running[i] = true;
                        Xi[i]      = am.value;
                        sigma[i]   = eq.slope(sub_level,Ci,Ki,xmul,xadd);
                        active << eq;
                        break;
                }

                if(xml.verbose)
                {
                    dom.eqfmt.pad( *xml << eq, eq) << ": " << am;
                    xml() << ", ma = " << std::setw(15) << eq.mass_action(sub_level,Corg,Ki,xmul);
                    xml() << ", sigma " << sigma[i] << std::endl;
                }
            }

            YACK_XMLOG(xml,"#active = " << active.size << " / " << dom.L << " / rank=" << dom.N);

            switch(active.size)
            {
                case 0: YACK_XMLOG(xml,"all blocked"); return;
                case 1: {
                    const equilibrium &eq = ***active.head;
                    YACK_XMLOG(xml,"unique " << eq);
                    iota::save(C0, Cs[eq.indx[sub_level]] );
                    return;
                }
                default:
                    break;

            }

            merge_list_of<eq_node>::sort(active,cmp);

            if(xml.verbose)
            {
                for(const eq_node *node=active.head;node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = eq.indx[sub_level];
                    dom.eqfmt.pad( *xml << eq, eq) << ": " << std::setw(15) << Xi[ei] << " " << dom.topo[ei] << std::endl;
                }
            }


        }

    }

}
