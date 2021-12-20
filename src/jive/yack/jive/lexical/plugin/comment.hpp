//! \file

#ifndef YACK_JIVE_LEXICAL_COMMENT_INCLUDED
#define YACK_JIVE_LEXICAL_COMMENT_INCLUDED 1

#include "yack/jive/lexical/plugin.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {

            class comment : public plugin
            {
            public:
                virtual ~comment() throw();

            protected:
                template <
                typename IDENTIFIER,
                typename EXPRESSION
                > inline
                explicit comment(const IDENTIFIER &uuid,
                                 const EXPRESSION &expr,
                                 analyzer         &host) :
                plugin(uuid,expr,host)
                {
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(comment);
            };

        }

    }
}

#endif
