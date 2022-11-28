
#include "yack/chem/reactor.hpp"
#include "yack/math/iota.hpp"
#include "yack/ios/xmlog.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        void reactor:: preservedGroup(writable<double> &C0,
                                      const rs_group   &rg,
                                      const xmlog      &xml)
        {
            YACK_XMLOG(xml,"-- enforcing " << rg);

            //------------------------------------------------------------------
            //
            // at most |rg| cycles
            //
            //------------------------------------------------------------------
            const size_t nc = rg.size;
            for(size_t cycle=1;cycle<=nc;++cycle)
            {

                double             gain = -1;
                const restriction *best = NULL;

                //--------------------------------------------------------------
                // look for the smallest positive gain
                //--------------------------------------------------------------
                for(const rs_node *rn=rg.head;rn;rn=rn->next)
                {
                    const restriction &rs = **rn;
                    const size_t       ri =  *rs; if(!Qb[ri]) continue; // already satisfied
                    const double       rv =  rs.apply(Qm[*rs],C0,xadd); // current gain
                    YACK_XMLOG(xml, "--  gain = " << std::setw(15) << rv << " @" << rs);
                    if(rv<=0)
                    {
                        // satisfied, remove from bad
                        Qb[*rs] = false;
                    }
                    else
                    {
                        // update
                        assert(rv>0);
                        if( gain<0 || rv<gain )
                        {
                            gain =  rv;
                            best = &rs;
                        }
                    }
                }

                if(!best)
                {
                    // all good for this group
                    break;
                }
                else
                {
                    // update status
                    YACK_XMLOG(xml, "--> using " << *best);
                    const size_t i = **best;
                    iota::load(C0,Qm[i]); // make new concentation
                    Qb[i] = false;        // remove from bad
                    Qg[i] = gain;         // store gain
                }
            }

            
        }

        double reactor:: preserved(writable<double> &C0, const xmlog &xml)
        {
            static const char fn[] = "preserving";
            YACK_XMLSUB(xml,fn);

            Qb.ld(true);
            Qg.ld(0);

            if(verbose)
            {
                iota::load(Ctry,C0);
            }

            for(const rs_group *rg=Qt.head;rg;rg=rg->next)
            {
                preservedGroup(C0,*rg,xml);
            }

            const double injected = xadd.tableau(Qg);
            if(verbose && injected)
            {
                *xml << "--> injected with " << injected << " M <--" << std::endl;
                for(const anode *an=working.head;an;an=an->next)
                {
                    const species &s = **an;
                    const size_t   j = *s;
                    corelib.pad(*xml << "|_[" << s.name << "]",s) << " = " << std::setw(15) << Ctry[j] << " --> " << std::setw(15) << C0[j] << std::endl;
                }
            }

            return injected;
        }

        
    }

}

