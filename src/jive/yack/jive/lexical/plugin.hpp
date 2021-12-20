//! \file

#ifndef YACK_JIVE_LEXICAL_PLUGIN_INCLUDED
#define YACK_JIVE_LEXICAL_PLUGIN_INCLUDED 1

#include "yack/jive/lexical/analyzer.hpp"

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
                scanner(label)
                {

                }

                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(plugin);
            };
        }

    }
}

#endif

