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


#define YACK_JIVE_PLUGIN_DECL(CLASS) static const type2type<CLASS> clid       //!< helper to declare plugin type
#define YACK_JIVE_PLUGIN_IMPL(CLASS) const type2type<CLASS> CLASS::clid = {}  //!< helper to implement plugin type

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
                virtual ~plugin() noexcept;

                //! setup with triggering expression
                template <
                typename IDENTIFIER,
                typename EXPRESSION>
                explicit plugin(const IDENTIFIER &uuid,
                                const EXPRESSION &expr,
                                analyzer         &host,
                                const eos_policy  flag) :
                scanner( uuid,flag ),
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
                /**
                 return lexeme for further manupulation if necessary
                 */
                //______________________________________________________________
                lexeme &inject(token &);

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(plugin);
                virtual void enter(token&) = 0;
            };

        }

    }
}

#endif

