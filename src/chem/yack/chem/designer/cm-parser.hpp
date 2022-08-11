

//! \file

#ifndef YACK_CHEMICAL_DESIGNER_CM_PARSER_INCLUDED
#define YACK_CHEMICAL_DESIGNER_CM_PARSER_INCLUDED 1


#include "yack/jive/parser.hpp"

namespace yack
{
    namespace chemical
    {
        namespace nucleus
        {
            class cm_parser : public jive::parser
            {
            public:
                virtual ~cm_parser() throw();
                explicit cm_parser();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(cm_parser);
            };
        }
    }

}

#endif
