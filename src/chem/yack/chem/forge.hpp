//! \file

#ifndef YACK_CHEM_FORGE_INCLUDED
#define YACK_CHEM_FORGE_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/components.hpp"
#include "yack/jive/parser.hpp"

namespace yack
{
    namespace chemical
    {
        class forge : public singleton<forge>
        {
        public:
            static const at_exit::longevity life_time = 10;
            static const char               call_sign[];

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

            auto_ptr<jive::parser> compiler;
            void create(components &, library &, jive::module *);
        };
    }
}

#endif

