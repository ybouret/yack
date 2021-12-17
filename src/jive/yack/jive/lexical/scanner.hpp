//! \file

#ifndef YACK_LEXICAL_SCANNER_INCLUDED
#define YACK_LEXICAL_SCANNER_INCLUDED 1

#include "yack/jive/lexical/instructions.hpp"
#include "yack/jive/lexical/flow-control.hpp"
#include "yack/jive/lexical/lexeme.hpp"
#include "yack/jive/pattern/scatter-table.hpp"
#include "yack/jive/pattern/regexp.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
           
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
                root(NULL),
                dict(NULL),
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
                    const motif  m(regexp::compile(expr,dict) );
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

                template <
                typename IDENTIFIER,
                typename EXPRESSION,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void jump(const IDENTIFIER &name,
                                 const EXPRESSION &expr,
                                 OBJECT_POINTER    host,
                                 METHOD_POINTER    meth)
                {
                    check_ctrl();
                    const tag    t_expr = tags::make( expr );
                    const tag    t_text = tags::make( flow::jump::text );
                    const tag    t_name = tags::make( name );
                    const string temp    = *t_expr + *t_text + *t_name;
                    const tag    t_jump = tags::make( temp );
                    
                    const flow::callback  cb(host,meth);
                    const flow::jump      fn(t_name,*ctrl_,cb);
                    const motif     m( regexp::compile(*t_expr,dict) );
                    const action    a(fn);
                    on(t_jump,m,a);
                }
                
                //______________________________________________________________
                //
                //! probe next lexeme
                /**

                 */
                //______________________________________________________________
                lexeme *probe(source &source, bool &ctrl);
                
                void     link_to(analyzer&) throw();
                void     restore(token&)    throw();
                
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag                             label; //!< scanner label
                const auto_ptr<const instructions>    instr; //!< set of instructions
                const auto_ptr<const scatter::table>  table; //!< scattered directives
                
                
            protected:
                source                                *root;
            
            private:
                const dictionary                      *dict;
                analyzer                              *ctrl_;
                YACK_DISABLE_COPY_AND_ASSIGN(scanner);
                void check_word(const tag &uuid, const token &word) const;
                void check_ctrl() const;
                
            public:
                bool verbose; //!< trigger verbositu
            };
        }

    }
}

#endif

