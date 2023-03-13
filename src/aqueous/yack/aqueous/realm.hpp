//! \file

#ifndef YACK_AQUEOUS_REALM_INCLUDED
#define YACK_AQUEOUS_REALM_INCLUDED 1

#include "yack/aqueous/realm/domain.hpp"
#include "yack/aqueous/library.hpp"

namespace yack
{
    namespace aqueous
    {

        class realm : public domains
        {
        public:
            const library    &lib;
            const equilibria &eqs;

            explicit realm(const library    &,
                           const equilibria &);
            virtual ~realm() noexcept;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(realm);
            void build_domains(const xmlog &);
        };

    }

}

#endif

