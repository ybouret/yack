
//! \file

#ifndef YACK_JIVE_SYNTAX_INTERNAL_INCLUDED
#define YACK_JIVE_SYNTAX_INTERNAL_INCLUDED 1

#include "yack/jive/syntax/rule.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            class internal : public rule
            {
            public:
                virtual ~internal()                            throw();

            protected:
                explicit internal(const tag &, const uint32_t) throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(internal);
            };
        }
    }
}


#endif

