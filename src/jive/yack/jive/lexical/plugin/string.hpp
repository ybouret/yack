//! \file

#ifndef YACK_JIVE_LEXICAL_STRING_INCLUDED
#define YACK_JIVE_LEXICAL_STRING_INCLUDED 1

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
            //! base class to create string
            //
            //__________________________________________________________________
            class string_ : public plugin
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~string_() throw(); //!< cleanup

                const char eos;
                const char raw;

            protected:
                //! setup with enter/leave char
                template <
                typename IDENTIFIER
                > inline
                explicit string_(const IDENTIFIER &uuid,
                                 const char        init,
                                 const char        done,
                                 const char        keep,
                                 analyzer         &host) :
                plugin(uuid,init,host),
                eos(done),
                raw(keep)
                {
                    setup();
                }
                
                token content;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(string_);
                void         setup();
                void         leave(token &);
                virtual void enter(token &) throw();
            };

        }

    }
}

#endif
