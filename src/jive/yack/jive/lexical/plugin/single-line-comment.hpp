
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
                comment(uuid,expr,host,accept_eos)
                {
                    compile();
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(single_line_comment);
                void         compile();
                void         leave(token &) throw();  //!< restore endl
                virtual void enter(token &) throw();  //!< token matches expr
            };

            //! helper for inline single line comment
#define YACK_JIVE_SINGLE_LINE_COMMENT(NAME,EXPR) \
/**/  class NAME : public single_line_comment\
/**/  {\
/**/   public:\
/**/    YACK_JIVE_PLUGIN_DECL(NAME);    \
/**/    inline virtual ~NAME() throw() {}\
/**/    template<typename IDENTIFIER>\
/**/    inline explicit NAME(const IDENTIFIER &uuid,analyzer &host) :\
/**/      single_line_comment(uuid,EXPR,host) {} \
/**/   private:\
/**/    YACK_DISABLE_COPY_AND_ASSIGN(NAME);\
/**/  }

            //! example C++ comment
            YACK_JIVE_SINGLE_LINE_COMMENT(cxx_comment,"//");

        }

    }
}

#endif
