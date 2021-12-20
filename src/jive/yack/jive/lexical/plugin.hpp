//! \file

#ifndef YACK_JIVE_LEXICAL_PLUGIN_INCLUDED
#define YACK_JIVE_LEXICAL_PLUGIN_INCLUDED 1

#include "yack/jive/lexical/scanner.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            class plugin : public scanner
            {
            public:
                virtual ~plugin() throw();

                template <
                typename IDENTIFIER,
                typename EXPRESSION>
                explicit plugin(const IDENTIFIER &uuid,
                                const EXPRESSION &expr,
                                analyzer         &host) :
                scanner(uuid),
                trigger( tags::make(expr) )
                {
                    link_to(host);
                }

                const tag trigger;
                void      enter(token &);

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(plugin);
                virtual void initialize(token&) = 0;
            };
        }

    }
}

#endif

