
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
            class terminal;

            //__________________________________________________________________
            //
            //
            //! grammar
            //
            //__________________________________________________________________
            class grammar : public ios::vizible
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef syntax::rule     rule; //!< alias

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

                //! terminal creation
                const rule &term__(const tag &id);

                //! create an optional rule from an existing rule
                const rule &opt_(const tag &id, const rule &r);

                //! opt_ with auto-naming
                const rule &opt(const rule &r);


                //! create a repeating rule from an existing rule
                const rule &rep_(const tag &id, const rule &r, const size_t n);

                //! rep_ with auto-naming
                const rule &rep(const rule &r, const size_t n);


                //! generic term
                template <typename ID> inline
                const rule &term_(const ID &id)
                {
                    const tag _ = tags::make(id); return term__(_);
                }


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
                const rule &zom(const rule &r);


                //! create a repeating>=1 rule from an existing rule
                const rule &oom(const rule &r);

                //! render grammar
                void gv() const;

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag lang; //!< language name

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(grammar);
                cxx_list_of<rule> rules;
                void       declare(rule *);
                void       gv_draw(ios::ostream &fp) const;

            };

        }

    }
}

#endif
