//! \file

#ifndef YACK_JSON_PARSER_INCLUDED
#define YACK_JSON_PARSER_INCLUDED 1

#include "yack/jive/parser.hpp"

namespace yack
{

    namespace JSON
    {

        //! parser
        class Parser : public jive::parser
        {
        public:
            explicit Parser();
            virtual ~Parser() throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }

}

#endif

