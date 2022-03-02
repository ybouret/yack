//! \file

#ifndef YACK_CHEM_FORGE_INCLUDED
#define YACK_CHEM_FORGE_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/components.hpp"
#include "yack/jive/parser.hpp"
#include "yack/jive/syntax/translator.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! helper to compile equilibria/species
        //
        //______________________________________________________________________
        class forge : public singleton<forge>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const at_exit::longevity life_time = 10; //!< singleton
            static const char               call_sign[];    //!< singleton

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! append to components/library the info
            template <typename INFO> inline
            void operator()(components &cmp,
                            library    &lib,
                            const INFO &inf)
            {
                create(cmp,lib,jive::module::open_data(inf));
            }

        private:
            friend class singleton<forge>;
            explicit forge();
            virtual ~forge() throw();

            auto_ptr<jive::parser>             parser;
            auto_ptr<jive::syntax::translator> linker;

            void create(components &, library &, jive::module *);
        };
    }
}

#endif

