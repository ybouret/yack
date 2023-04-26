//! \file

#ifndef YACK_CSV_INCLUDED
#define YACK_CSV_INCLUDED

#include "yack/jive/parser.hpp"

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
        };
    }

}

#endif


