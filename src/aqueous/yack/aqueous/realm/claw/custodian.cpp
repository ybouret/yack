
#include "yack/aqueous/realm/claw/custodian.hpp"
#include <iomanip>

namespace yack
{
    namespace aqueous
    {
        custodian:: ~custodian() noexcept
        {
        }

        custodian:: custodian(const size_t M) :
        injected(M)
        {
        }

        void custodian:: prepare() noexcept
        {
            injected.ld(0);
        }

        void custodian:: process(const xmlog      &xml,
                                 writable<double> &C,
                                 const conserved  &laws)
        {
            YACK_XMLSUB(xml,"custodian::process::laws");

            broken.clear();
            excess.clear();

            YACK_XMLOG(xml,"|laws|=" << laws.size);
            //------------------------------------------------------------------
            //
            // initialize broken/excess
            //
            //------------------------------------------------------------------
            for(const conserved::node_type *node=laws.head;node;node=node->next)
            {
                const conservation &law = ***node;
                const double        cxs = law.excess(C,xadd);
                if(cxs<0)
                {
                    broken << law;
                    excess << cxs;
                    YACK_XMLOG(xml, std::setw(15) << cxs << " @" << law);
                }
            }

            //------------------------------------------------------------------
            //
            // select law with smallest excess
            //
            //------------------------------------------------------------------
        FIX:
            YACK_XMLOG(xml, "|broken| = " << broken.size << " / " << laws.size);
            if(broken.size<=0) { assert(0==excess.size); return; }

            //------------------------------------------------------------------
            // initialize look up
            //------------------------------------------------------------------
            broken_node *brn = broken.head;
            excess_node *xsn = excess.head;
            double       opt = **xsn; assert(opt<0);

            //------------------------------------------------------------------
            // selecting node
            //------------------------------------------------------------------
            excess_node     *xst = xsn->next;
            for(broken_node *brt = brn->next;brt;brt=brt->next,xst=xst->next)
            {
                const double temp = **xst; assert(temp<0);
                if(temp>opt)
                {
                    opt = temp;
                    brn = brt;
                    xsn = xst;
                }
            }

            //------------------------------------------------------------------
            // process winner
            //------------------------------------------------------------------
            const conservation &law = ***brn;
            const double        q   = -opt;

            YACK_XMLOG(xml,"(*) " << std::setw(15) << opt << " @" << law);

            const double den = law.nrm2;
            for(const actor *a=law.head;a;a=a->next)
            {
                const species &sp = **a;
                const size_t   sj = sp.indx[top_level];
                const double   dc = (q*a->nu)/den;
                C[sj]        += dc;
                injected[sj] += dc;
            }

            //------------------------------------------------------------------
            // winner is done, won't come back
            //------------------------------------------------------------------
            broken.cut(brn);
            excess.clear();

            //------------------------------------------------------------------
            // rebuild list from remaining broken
            //------------------------------------------------------------------
            {
                core_repo<const conservation> filter;
                while(broken.size)
                {
                    broken_node        *node = broken.pop_front();
                    const conservation &cons = ***node;
                    const double        temp = cons.excess(C,xadd);
                    if(temp<0)
                    {
                        YACK_XMLOG(xml, "(+) " << std::setw(15) << temp << " @" << cons);
                        filter.push_back(node);
                        excess << temp;
                    }
                    else
                    {
                        YACK_XMLOG(xml, "(-) " << std::setw(15) << ' ' << " @" << cons);
                        broken.cache->ingest(node);
                    }
                }
                broken.swap_with(filter);
            }
            goto FIX;
        }



    }


}

