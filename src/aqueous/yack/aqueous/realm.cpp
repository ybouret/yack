
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
        lib(lib_),
        eqs(eqs_)
        {
            xmlog xml("[realm]",std::cerr,species::verbose);

            // make partition
            build_domains(xml);

            // create partitions
            for(domain *dom=head;dom;dom=dom->next)
            {
                dom->create(xml,lib,eqs,eks);
            }
        }

       
    }
    
}

