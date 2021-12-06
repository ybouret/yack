//! \file

#ifndef YACK_JIVE_REGEXP_INCLUDED
#define YACK_JIVE_REGEXP_INCLUDED 1

#include "yack/jive/pattern.hpp"

namespace yack
{
    namespace jive
    {
        class dictionary;
        
        //! REGular EXPression compiler
        struct regexp
        {
            static pattern *compile(const string &expr, const dictionary *dict=NULL);
        };
    }

}

#endif

