
//! \file

#ifndef YACK_AQUEOUS_DOMAIN_INCLUDED
#define YACK_AQUEOUS_DOMAIN_INCLUDED 1

#include "yack/aqueous/equilibria.hpp"
#include "yack/data/dinky/core-list.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack
{
    namespace aqueous
    {

        class domain : public large_object, public eq_list
        {
        public:
            domain       *next;
            domain       *prev;

            explicit  domain(const equilibrium &first);
            virtual  ~domain() noexcept;

            bool      accepts(const equilibrium &) const noexcept;
            bool      accepts(const domain      &) const noexcept;

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(domain);
        };

        typedef cxx_list_of<domain> domains;

    }

}

#endif

