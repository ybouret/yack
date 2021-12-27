
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

                //! create an optional rule from an existing rule
                const rule &opt_(const tag &id, const rule &r);

                //! create a repeating rule from an existing rule
                const rule &rep_(const tag &id, const rule &r, const size_t n);

                //! create an optional rule from an existing rule
                template <typename ID> inline
                const rule &opt(const ID &id, const rule &r)
                {
                    const tag _ = tags::make(id); return opt_(_,r);
                }

                //! create a repeating rule from an existing rule
                template <typename ID> inline
                const rule &rep(const ID &id, const rule &r, const size_t n)
                {
                    const tag _ = tags::make(id); return rep(_,r,n);
                }

                //! create a repeating>=0 rule from an existing rule
                template <typename ID> inline
                const rule &zom(const ID &id, const rule &r)
                {
                    return rep(id,r,0);
                }

                //! create a repeating>=1 rule from an existing rule
                template <typename ID> inline
                const rule &oom(const ID &id, const rule &r)
                {
                    return rep(id,r,1);
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
