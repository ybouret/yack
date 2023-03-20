//! \file

#ifndef YACK_AQUEOUS_REALM_INCLUDED
#define YACK_AQUEOUS_REALM_INCLUDED 1

#include "yack/aqueous/realm/domain.hpp"
#include "yack/aqueous/library.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! chemical system
        //
        //______________________________________________________________________
        class realm : public domains, public xmlog
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit realm(const library          &,
                           const equilibria       &,
                           const readable<double> &); //!< setup
            virtual ~realm() noexcept;          //!< cleanup


          

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library        &lib; //!< persistent library
            equilibria            eqs; //!< original persistent equilibrium
            const cxx_array<bool> reg; //!< regular species

            void graphviz(const string &filename) const;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(realm);

            //! group equilibria in domains
            void build_domains(const xmlog &);
        };

    }

}

#endif

