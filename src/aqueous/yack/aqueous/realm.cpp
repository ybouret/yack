
#include "yack/aqueous/realm.hpp"

namespace yack
{
    namespace aqueous
    {
        realm:: ~realm() noexcept
        {
        }

        realm:: realm(const library    &lib_,
                      const equilibria &eqs_) :
        domains(),
        lib(lib_),
        eqs(eqs_)
        {
            xmlog xml("[realm]",std::cerr,species::verbose);
            build_domains(xml);
            for(domain *dom=head;dom;dom=dom->next)
            {
                dom->create(xml);
            }
        }

       
    }
    
}

