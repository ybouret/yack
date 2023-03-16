
#include "yack/aqueous/realm.hpp"

namespace yack
{
    namespace aqueous
    {
        realm:: ~realm() noexcept
        {
        }

        realm:: realm(const library          &lib_,
                      const equilibria       &eqs_,
                      const readable<double> &eks) :
        domains(),
        xmlog("[realm]",std::cerr,species::verbose),
        lib(lib_),
        eqs(eqs_),
        reg(lib->size,true)
        {
            const xmlog &xml = *this;
            //------------------------------------------------------------------
            //
            // make partition of independents equilibria
            //
            //------------------------------------------------------------------
            build_domains(xml);

            //------------------------------------------------------------------
            //
            // create partitions
            //
            //------------------------------------------------------------------
            for(domain *dom=head;dom;dom=dom->next)
            {
                dom->create(xml,eqs,eks);
            }

            lib(std::cerr << "reg=",reg);
        }


    }
    
}

#include "yack/aqueous/realm/claw/custodian.hpp"

namespace yack
{
    namespace aqueous
    {
        void custodian:: process( writable<double> &C,
                                 const realm      &cs)
        {
            const xmlog &xml = cs;
            YACK_XMLSUB(xml,"custodian::process");
            prepare();
            for(const domain *dom = cs.head;dom;dom=dom->next)
            {
                for(const conserved *cns=dom->pack.head;cns;cns=cns->next)
                {
                    process(xml,C,*cns);
                }
            }
        }
    }
}





