
//! \file

#ifndef YACK_JIVE_SYNTAX_GRAMMAR_INCLUDED
#define YACK_JIVE_SYNTAX_GRAMMAR_INCLUDED 1

#include "yack/jive/syntax/rule/compound.hpp"
#include "yack/jive/syntax/rule/term-role.hpp"
#include "yack/jive/syntax/rule/agg-role.hpp"


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
                typedef syntax::rule     rule;     //!< alias
                typedef syntax::compound compound; //!< alias
                

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~grammar() throw();       //!< cleanup

                //! setup
                template <typename LANG> inline
                explicit grammar(const LANG &id) :
                lang( tags::make(id) ), rules()
                {
                }

                //______________________________________________________________
                //
                // construction methods
                //______________________________________________________________

                //! declare a new (derived) rule
                template <typename RULE>
                RULE &decl(RULE *r)
                {
                    assert(r);
                    declare(r);
                    return *r;
                }

                //______________________________________________________________
                //
                // terminal
                //______________________________________________________________

                //! terminal creation
                const rule &term__(const tag &id, const term_role role);

                //! generic term
                template <typename ID> inline
                const rule &term_(const ID &id, const term_role role=standard)
                {
                    const tag _ = tags::make(id); return term__(_,role);
                }

                //______________________________________________________________
                //
                // option
                //______________________________________________________________

                //! create an optional rule from an existing rule
                const rule &opt_(const tag &id, const rule &r);

                //! opt_ with auto-naming
                const rule &opt(const rule &r);


                //! create an optional rule from an existing rule
                template <typename ID> inline
                const rule &opt(const ID &id, const rule &r)
                {
                    const tag _ = tags::make(id); return opt_(_,r);
                }

                //______________________________________________________________
                //
                // repeat
                //______________________________________________________________
                //! create a repeating rule from an existing rule
                const rule &rep_(const tag &id, const rule &r, const size_t n);

                //! rep_ with auto-naming
                const rule &rep(const rule &r, const size_t n);

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

                //______________________________________________________________
                //
                // aggregate
                //______________________________________________________________

                //! create an aggregate
                compound &agg_(const tag     &name,
                               const agg_role role);

                //! create a (default) named rule
                template <typename ID> inline
                compound &agg(const ID &id)
                {
                    const tag _ = tags::make(id); return agg_(_,syntax::named);
                }

                //! create a group rule (only transient)
                template <typename ID> inline
                compound &grp(const ID &id)
                {
                    const tag _ = tags::make(id); return agg_(_,syntax::group);
                }

                //! create a proxy rule
                template <typename ID> inline
                compound &act(const ID &id)
                {
                    const tag _ = tags::make(id); return agg_(_,syntax::proxy);
                }
                


                //! (a,b)
                const rule &cat(const rule &a, const rule &b);

                //! (a,b,c)
                const rule &cat(const rule &a, const rule &b, const rule &c);


                //______________________________________________________________
                //
                // alternate
                //______________________________________________________________
                //! create and alternate
                compound &alt_(const tag &name);

                //! create an optional rule from an existing rule
                template <typename ID> inline
                compound &alt(const ID &id)
                {
                    const tag _ = tags::make(id); return alt_(_);
                }

                //! (a|b)
                const rule &choice(const rule &a, const rule &b);

                //! (a|b|c)
                const rule &choice(const rule &a, const rule &b, const rule &c);

                //______________________________________________________________
                //
                // access
                //______________________________________________________________

                //! return current list of rules
                const list_of<rule> & operator*() const throw();

                //! set top level rule
                void top(const rule &);

                //! get top level rult
                const rule *top() const throw();
                
                //! get terminal by name
                const terminal & operator[](const string &) const;



                //______________________________________________________________
                //
                // other methods
                //______________________________________________________________

                //! render grammar
                void gv() const;

                //! validate grammar
                void validate() const;

                //! collect keywords
                void collect_keywords(sequence<string> &terminals,
                                      sequence<string> &internals) const;

                //! emit some keywordds
                static void emit_keywords(ios::ostream           &fp,
                                          const readable<string> &table,
                                          const string           &cxx_id,
                                          const string           &prefix);

                //______________________________________________________________
                //
                //! run is accepting first rule
                //______________________________________________________________
                xnode *run(source &, lexer &) const;
                

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
                void       must_own(const rule &a, const char *fn) const;
            };


        }

    }
}

#endif
