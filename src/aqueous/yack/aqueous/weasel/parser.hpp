//! \file

#ifndef YACK_AQUEOUSE_WEASEL_PARSER_INCLUDED
#define YACK_AQUEOUSE_WEASEL_PARSER_INCLUDED 1

#include "yack/jive/parser.hpp"
#include "yack/jive/syntax/xnode.hpp"

namespace yack
{
    namespace aqueous
    {
        //! Weasel Exerts A Species/Equilibrium Language
        namespace weasel
        {
            typedef jive::syntax::xnode xnode;

            class parser : public jive::parser
            {
            public:
                explicit parser();
                virtual ~parser() noexcept;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(parser);
            };
        }
    }
}

#endif

