
//! \file

#ifndef YACK_JIVE_SYNTAX_INTERNAL_INCLUDED
#define YACK_JIVE_SYNTAX_INTERNAL_INCLUDED 1

#include "yack/jive/syntax/rule.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            //__________________________________________________________________
            //
            //
            //! base class for internal rules
            //
            //__________________________________________________________________
            class internal : public rule
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~internal()                            noexcept; //!< cleanup

            protected:
                explicit internal(const tag &, const uint32_t) noexcept; //!< set name/uuid

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(internal);
            };
        }
    }
}


#endif

