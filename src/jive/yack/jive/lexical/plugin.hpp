//! \file

#ifndef YACK_JIVE_LEXICAL_PLUGIN_INCLUDED
#define YACK_JIVE_LEXICAL_PLUGIN_INCLUDED 1

#include "yack/jive/lexical/scanner.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            //__________________________________________________________________
            //
            //
            //! a plugin is a precompile scanner
            //
            //__________________________________________________________________
            class plugin : public scanner
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~plugin() throw();

                //! setup with triggering expression
                template <
                typename IDENTIFIER,
                typename EXPRESSION>
                explicit plugin(const IDENTIFIER &uuid,
                                const EXPRESSION &expr,
                                analyzer         &host) :
                scanner(uuid),
                trigger( tags::make(expr) )
                {
                    link_to(host);
                }


                //______________________________________________________________
                //
                // method
                //______________________________________________________________
                void         on_call(token &); //!< on trigger

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag    trigger;          //!< expression as tag

            protected:
                //______________________________________________________________
                //
                //! store a new lexeme built from label and non-empty token
                //______________________________________________________________
                void inject(token &) const;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(plugin);
                virtual void enter(token&) = 0;
            };
        }

    }
}

#endif

