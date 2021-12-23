

//! \file

#ifndef YACK_JIVE_SYNTAX_INNER_INCLUDED
#define YACK_JIVE_SYNTAX_INNER_INCLUDED 1

#include "yack/jive/syntax/rule/internal.hpp"
#include "yack/type/fourcc.h"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            class inner : public internal
            {
            public:
                static const uint32_t mark = YACK_FOURCC('I', 'N', 'N', 'R');

                virtual ~inner()           throw();
                explicit inner(const tag &) throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(inner);
            };
        }
    }
}


#endif

