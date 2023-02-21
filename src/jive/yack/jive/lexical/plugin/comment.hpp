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

            //__________________________________________________________________
            //
            //
            //! base class to create comments
            //
            //__________________________________________________________________
            class comment : public plugin
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~comment() noexcept; //!< cleanup

            protected:

                //! setup with triggering expression
                template <
                typename IDENTIFIER,
                typename EXPRESSION
                > inline
                explicit comment(const IDENTIFIER &uuid,
                                 const EXPRESSION &expr,
                                 analyzer         &host,
                                 const eos_policy  flag) :
                plugin(uuid,expr,host,flag)
                {
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(comment);
            };

        }

    }
}

#endif
