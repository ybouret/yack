
//! \file

#ifndef YACK_JIVE_LEXICAL_ANALYZER_INCLUDED
#define YACK_JIVE_LEXICAL_ANALYZER_INCLUDED 1

#include "yack/jive/lexical/scanner.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {

            class analyzer : public scanner
            {
            public:
                virtual ~analyzer() throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(analyzer);

            };

        }

    }

}

#endif

