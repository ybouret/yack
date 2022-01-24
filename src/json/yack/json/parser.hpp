//! \file

#ifndef YACK_JSON_PARSER_INCLUDED
#define YACK_JSON_PARSER_INCLUDED 1

#include "yack/json/value.hpp"
#include "yack/jive/parser.hpp"
#include "yack/jive/syntax/translator.hpp"

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

            void operator()(jive::module *);
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Parser);
            auto_ptr<jive::syntax::translator> tr;
        };

    }

}

#endif

