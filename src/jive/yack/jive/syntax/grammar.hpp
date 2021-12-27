
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
                RULE &decl(RULE *r)
                {
                    assert(r);
                    declare(r);
                    return *r;
                }

                const rule &opt(const tag &id, const rule &r);
                const rule &rep(const tag &id, const rule &r, const size_t n);
                const rule &zom(const tag &id, const rule &r);
                const rule &oom(const tag &id, const rule &r);

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
