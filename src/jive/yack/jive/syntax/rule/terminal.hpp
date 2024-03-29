//! \file

#ifndef YACK_JIVE_SYNTAX_TERMINAL_INCLUDED
#define YACK_JIVE_SYNTAX_TERMINAL_INCLUDED 1

#include "yack/jive/syntax/rule.hpp"
#include "yack/jive/syntax/rule/term-role.hpp"
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
            //! a terminal producing rule
            //
            //__________________________________________________________________
            class terminal : public rule
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const uint32_t mark = YACK_FOURCC('T', 'E', 'R', 'M'); //!< UUID


                //______________________________________________________________
                //
                // interface
                //______________________________________________________________
                virtual bool accept(YACK_JIVE_RULE_ARGS) const;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~terminal()            noexcept; //!< cleanup
                explicit terminal(const tag &, const term_role) noexcept; //!< setup name

                //______________________________________________________________
                //
                // member
                //______________________________________________________________
                const term_role role; //!< terminal classification

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(terminal);
            };
        }
    }
}


#endif

