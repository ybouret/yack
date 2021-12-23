//! \file

#ifndef YACK_JIVE_SYNTAX_XNODE_INCLUDED
#define YACK_JIVE_SYNTAX_XNODE_INCLUDED 1

#include "yack/jive/lexical/lexeme.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            class xnode : public object
            {
            public:
                virtual ~xnode() throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(xnode);
            };
        }
    }
}

#endif

