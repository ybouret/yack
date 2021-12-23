//! \file

#ifndef YACK_JIVE_SYNTAX_RULE_INCLUDED
#define YACK_JIVE_SYNTAX_RULE_INCLUDED 1

#include "yack/jive/lexical/analyzer.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            enum family
            {
                is_terminal,
                is_internal
            };

            class rule : public object
            {
            public:
                virtual ~rule() throw();

                const tag    uuid;
                const family kind;

            protected:
                explicit rule(const tag &id, const family &fm) throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rule);
            };
        }
    }
}

#endif

