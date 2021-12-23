//! \file

#ifndef YACK_JIVE_SYNTAX_RULE_INCLUDED
#define YACK_JIVE_SYNTAX_RULE_INCLUDED 1

#include "yack/jive/syntax/xnode.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            class rule
            {
            public:
                virtual ~rule() throw();


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rule);
            };
        }
    }
}

#endif

