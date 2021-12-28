
//! \file

#ifndef YACK_JIVE_SYNTAX_ALTERNATE_INCLUDED
#define YACK_JIVE_SYNTAX_ALTERNATE_INCLUDED 1

#include "yack/jive/syntax/rule/compound.hpp"
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
            //! alternation of rules
            //
            //__________________________________________________________________
            class alternate : public compound
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const uint32_t mark = YACK_FOURCC('A','L','T',':'); //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~alternate()                 throw(); //!< cleanup
                explicit alternate(const tag &name_) throw(); //!< setup

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________
                virtual bool accept(YACK_JIVE_RULE_ARGS) const;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(alternate);
            };

        }

    }

}

#endif
