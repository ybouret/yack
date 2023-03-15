
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
        eqs(eqs_)
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
        }

       
    }
    
}

