//! \file

#ifndef YACK_JIVE_SYNTAX_TERMINAL_INCLUDED
#define YACK_JIVE_SYNTAX_TERMINAL_INCLUDED 1

#include "yack/jive/syntax/rule.hpp"
#include "yack/type/fourcc.h"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            class terminal : public rule
            {
            public:
                static const uint32_t mark = YACK_FOURCC('T', 'E', 'R', 'M');

                virtual ~terminal()            throw();
                explicit terminal(const tag &) throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(terminal);
            };
        }
    }
}


#endif

