

//! \file

#ifndef YACK_JIVE_LEXICAL_MULTI_LINES_COMMENT_INCLUDED
#define YACK_JIVE_LEXICAL_MULTI_LINES_COMMENT_INCLUDED 1

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
            //! multiple lines comment
            //
            //__________________________________________________________________
            class multi_lines_comment : public comment
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~multi_lines_comment() throw();

                //! setup between init/done expressions
                template <
                typename IDENTIFIER,
                typename EXPRESSINI,
                typename EXPRESSEND
                > inline
                explicit multi_lines_comment(const IDENTIFIER &uuid,
                                             const EXPRESSINI &init,
                                             const EXPRESSEND &done,
                                             analyzer         &host) :
                comment(uuid,init,host)
                {
                    back(done,this,&multi_lines_comment::leave);
                    compile();
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(multi_lines_comment);
                void         compile();               //!< create directives
                void         leave(token &) throw();  //!< drop done expression
                virtual void enter(token &) throw();  //!< drop init expression
            };

        }

    }
}

#endif
