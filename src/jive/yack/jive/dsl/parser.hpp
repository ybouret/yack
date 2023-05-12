//! \file

#ifndef YACK_JIVE_DSL_PARSER_INCLUDED
#define YACK_JIVE_DSL_PARSER_INCLUDED 1

#include "yack/jive/parser.hpp"

namespace yack
{
    namespace jive
    {

        class dsl_parser : public parser
        {
        public:
            explicit dsl_parser();
            virtual ~dsl_parser() noexcept;
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(dsl_parser);
        };

    }


}

#endif

