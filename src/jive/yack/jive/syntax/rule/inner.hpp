

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
            //__________________________________________________________________
            //
            //
            //! demo internal rule
            //
            //__________________________________________________________________
            class inner : public internal
            {
            public:
                //______________________________________________________________
                //
                // definition
                //______________________________________________________________
                static const uint32_t mark = YACK_FOURCC('I', 'N', 'N', 'R'); //!< UUID

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________
                virtual bool accept(YACK_JIVE_RULE_ARGS) const; //!< false
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~inner()            noexcept; //!< cleanup
                explicit inner(const tag &) noexcept; //!< setup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(inner);
            };
        }
    }
}


#endif

