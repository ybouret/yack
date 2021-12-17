
#ifndef YACK_JIVE_LEXICAL_BEHAVIOR_INCLUDED
#define YACK_JIVE_LEXICAL_BEHAVIOR_INCLUDED 1

#include "yack/jive/pattern.hpp"
#include "yack/functor.hpp"

namespace yack
{
    namespace jive
    {
        namespace lexical
        {
            //__________________________________________________________________
            //
            //
            //! behavior when a matching directive is found
            //
            //__________________________________________________________________
            enum behavior
            {
                produce,
                discard,
                control
            };

        }
        
    }
}

#endif
