
//! \file

#ifndef YACK_CHEMICAL_DESIGNER_SP_PARSER_INCLUDED
#define YACK_CHEMICAL_DESIGNER_SP_PARSER_INCLUDED 1


#include "yack/jive/parser.hpp"

namespace yack
{
    namespace chemical
    {
        namespace nucleus
        {
            class sp_parser : public jive::parser
            {
            public:
                virtual ~sp_parser() throw();
                explicit sp_parser();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sp_parser);
            };
        }
    }

}

#endif
