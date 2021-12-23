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

                const tag      name;
                const family   kind;
                const uint32_t uuid;
                rule          *next;
                rule          *prev;
                
            protected:
                explicit rule(const tag     &name_,
                              const family   kind_,
                              const uint32_t uuid_ ) throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rule);
            };
        }
    }
}

#endif

