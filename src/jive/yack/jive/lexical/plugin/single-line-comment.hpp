
//! \file

#ifndef YACK_JIVE_LEXICAL_SINGLE_LINE_COMMENT_INCLUDED
#define YACK_JIVE_LEXICAL_SINGLE_LINE_COMMENT_INCLUDED 1

#include "yack/jive/lexical/plugin/comment.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {

            //__________________________________________________________________
            //
            //
            //! create single line comment
            //
            //__________________________________________________________________
            class single_line_comment : public comment
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~single_line_comment() throw(); //!< cleanup

                //! setup with triggering expression
                template <
                typename IDENTIFIER,
                typename EXPRESSION
                > inline
                explicit single_line_comment(const IDENTIFIER &uuid,
                                             const EXPRESSION &expr,
                                             analyzer         &host) :
                comment(uuid,expr,host)
                {
                    compile();
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(single_line_comment);
                void         compile();
                void         leave(token &) throw();  //!< restore endl
                virtual void enter(token &) throw();  //!< token matches expr
            };

        }

    }
}

#endif
