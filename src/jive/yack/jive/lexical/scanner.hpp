//! \file

#ifndef YACK_LEXICAL_SCANNER_INCLUDED
#define YACK_LEXICAL_SCANNER_INCLUDED 1

#include "yack/jive/lexical/instructions.hpp"
#include "yack/jive/lexical/lexeme.hpp"
#include "yack/jive/pattern/scatter-table.hpp"
#include "yack/jive/pattern/regexp.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            class analyzer;

            //__________________________________________________________________
            //
            //
            //! lexical scanner to produce lexemes
            //
            //__________________________________________________________________
            class scanner : public object, public counted
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~scanner() throw(); //!< cleanup

                //! setup with optional dictionary
                template <typename LABEL> inline
                explicit scanner(const LABEL &id) :
                object(),
                counted(),
                label( tags::make(id)       ),
                instr( new instructions()   ),
                table( new scatter::table() ),
                dict_(NULL),
                curr_(NULL),
                ctrl_(NULL),
                verbose(false)
                {}

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! return *tag
                const string & key() const throw();


                //! register a new directive
                void on(const tag &t, const motif &m, const action &a);

                //! generic helper to build directive
                template <
                typename IDENTIFIER,
                typename EXPRESSION,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void make(const IDENTIFIER     &uuid,
                                 const EXPRESSION     &expr,
                                 const OBJECT_POINTER  host,
                                 const METHOD_POINTER  meth)
                {
                    assert(host);
                    assert(meth);
                    const tag    t = tags::make(uuid);
                    const action a(host,meth);
                    const motif  m(regexp::compile(expr,dict_) );
                    on(t,m,a);
                }

                //! generic helper to build directive
                template <
                typename EXPRESSION,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void make(const EXPRESSION     &expr,
                                 const OBJECT_POINTER  host,
                                 const METHOD_POINTER  meth)
                {
                    make(expr,expr,host,meth);
                }


                //! helper to emit/produce uuid
                template <
                typename IDENTIFIER,
                typename EXPRESSION
                >
                inline void emit(const IDENTIFIER     &uuid,
                                 const EXPRESSION     &expr)
                {
                    make(uuid,expr,this,&scanner::on_produce);
                }

                //! helper to drop/discard uuid
                template <
                typename IDENTIFIER,
                typename EXPRESSION
                >
                inline void drop(const IDENTIFIER     &uuid,
                                 const EXPRESSION     &expr)
                {
                    make(uuid,expr,this,&scanner::on_discard);
                }

                //! helper to drop/discard expr=uuid
                template <typename EXPRESSION>
                inline void drop(const EXPRESSION     &expr)
                {
                    make(expr,expr,this,&scanner::on_discard);
                }


                //! helper to newline
                template <
                typename IDENTIFIER,
                typename EXPRESSION
                >
                inline void endl(const IDENTIFIER     &uuid,
                                 const EXPRESSION     &expr)
                {
                    make(uuid,expr,this,&scanner::on_newline);
                }

                //! helper to newline
                template <typename EXPRESSION>
                inline void endl(const EXPRESSION &expr)
                {
                    endl(expr,expr);
                }


                behavior on_produce(const token &) throw(); //!< default produce method: return produce
                behavior on_discard(const token &) throw(); //!< default discard method: return discard
                behavior on_newline(const token &) throw(); //!< curr_->newline()    and return discard

                friend std::ostream & operator<<(std::ostream &, const scanner &); //!< output quoted label

                //______________________________________________________________
                //
                //! probe next lexeme
                /**

                 */
                //______________________________________________________________
                lexeme *probe(source &source, bool &ctrl);

                void    linked_to(analyzer&) throw();
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag                             label; //!< scanner label
                const auto_ptr<const instructions>    instr; //!< set of instructions
                const auto_ptr<const scatter::table>  table; //!< scattered directives
                
            private:
                const dictionary                      *dict_;
                module                                *curr_;
                analyzer                              *ctrl_;
                YACK_DISABLE_COPY_AND_ASSIGN(scanner);
                void check_token(const tag &uuid, const token &word) const;
                
            public:
                bool verbose; //!< trigger verbositu
            };
        }

    }
}

#endif

