
//! \file

#ifndef YACK_JIVE_SYNTAX_GRAMMAR_INCLUDED
#define YACK_JIVE_SYNTAX_GRAMMAR_INCLUDED 1

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
            //! grammar
            //
            //__________________________________________________________________
            class grammar
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~grammar() throw(); //!< cleanup

                //! setup
                template <typename LANG> inline
                explicit grammar(const LANG &id) :
                lang( tags::make(id) ), rules()
                {
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! declare a new (derived) rule
                template <typename RULE>
                RULE &toto(RULE *r)
                {
                    assert(r);
                    declare(r);
                    return *r;
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag lang; //!< language name

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(grammar);
                cxx_list_of<rule> rules;
                void       declare(rule *);

            };

        }

    }
}

#endif
