//! \file

#ifndef YACK_AQUEOUSE_WEASEL_PARSER_INCLUDED
#define YACK_AQUEOUSE_WEASEL_PARSER_INCLUDED 1

#include "yack/jive/parser.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace weasel
        {
            
            //! Weasel Exerts A Species/Equilibrium Language
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

