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
        
        //______________________________________________________________________
        //
        //
        //! JSON parser
        //
        //______________________________________________________________________
        class Parser : public jive::parser
        {
        public:
            explicit Parser();         //!< setup
            virtual ~Parser() noexcept; //!< cleanup

            Value &operator()(jive::module *); //!< return *this after parsing

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Parser);
            auto_ptr<jive::syntax::translator> tr;
            Value                              jv;
        };

    }

}

#endif

