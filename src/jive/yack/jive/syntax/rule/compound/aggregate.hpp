//! \file

#ifndef YACK_JIVE_SYNTAX_AGGREGATE_INCLUDED
#define YACK_JIVE_SYNTAX_AGGREGATE_INCLUDED 1

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
            //! aggregate of rules
            //
            //__________________________________________________________________
            class aggregate : public compound
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const uint32_t mark = YACK_FOURCC('A','G','G',':'); //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~aggregate() throw();                 //!< cleanup
                explicit aggregate(const tag &name_) throw(); //!< setup

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

                //! must accept all
                virtual bool accept(YACK_JIVE_RULE_ARGS) const;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(aggregate);
            };

        }

    }

}

#endif
