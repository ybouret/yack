//! \file

#ifndef YACK_CSV_INCLUDED
#define YACK_CSV_INCLUDED

#include "yack/jive/parser.hpp"
#include "yack/jive/syntax/translator.hpp"

namespace yack
{

    namespace CSV
    {
        class Parser : public jive::parser
        {
        public:
            explicit Parser();
            virtual ~Parser() noexcept;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Parser);
            auto_ptr<jive::syntax::translator> tr;
        };
    }

}

#endif


