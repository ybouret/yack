
//! \file

#ifndef YACK_JIVE_SYNTAX_GRAMMAR_INCLUDED
#define YACK_JIVE_SYNTAX_GRAMMAR_INCLUDED 1

#include "yack/jive/syntax/rule.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            //__________________________________________________________________
            //
            //
            //! grammar
            //
            //__________________________________________________________________
            class grammar
            {
            public:
                virtual ~grammar() throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(grammar);
            };

        }

    }
}

#endif
