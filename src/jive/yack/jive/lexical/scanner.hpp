//! \file

#ifndef YACK_LEXICAL_SCANNER_INCLUDED
#define YACK_LEXICAL_SCANNER_INCLUDED 1

#include "yack/jive/lexical/instructions.hpp"
#include "yack/jive/lexical/flow/jump.hpp"
#include "yack/jive/lexical/flow/call.hpp"
#include "yack/jive/lexical/flow/back.hpp"
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
            //! End Of Stream policy
            //__________________________________________________________________
            enum eos_policy
            {
                accept_eos, //!< scanner may  stop at EOS
                reject_eos  //!< unauthorized stop at EOS
            };


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
                // types and definition
                //______________________________________________________________
                static size_t output_width; //!< verbose output names alignment
                static bool   verbose;      //!< class wise verbosity

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~scanner() throw(); //!< cleanup

                //! setup with default policy for standalone scanner
                template <typename LABEL> inline
                explicit scanner(const LABEL     &label_,
                                 const eos_policy flag=accept_eos) :
                object(),
                counted(),
                label( tags::make(label_)   ),
                instr( new instructions()   ),
                table( new scatter::table() ),
                onEOS(flag),
                flux(NULL),
                root(NULL),
                indx( &indx_ ),
                mydb(NULL),
                indx_(0)
                {
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! return *tag
                const string & key() const throw();


                //! register a new directive
                /**
                 with label 't',
                 carry out action 'a' if motif 'm' is selected
                 */
                void on(const tag &t, const motif &m, const action &a);

                //! generic helper to build directive
                /**
                 - create a directive 'uuid'
                 - when expression    'expr' is mest
                 - using a class pointer 'host'
                 - and a class method    'meth'
                    as actiont to perform
                 */
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
                    const motif  m(regexp::compile(expr,mydb) );
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

                friend std::ostream & operator<<(std::ostream &, const scanner &); //!< output quoted directives

                //! create a jump
                /**
                 - to scanner 'name'
                 - when found 'expr'
                 - and performing callback host.meth(word)
                 */
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
                    check_root();
                    const tag    t_expr = tags::make( expr );
                    const tag    t_text = tags::make( flow::jump::text );
                    const tag    t_name = tags::make( name );
                    const string temp   = *t_expr + *t_text + *t_name;
                    const tag    t_jump = tags::make( temp );
                    
                    const flow::callback  cb(host,meth);
                    const flow::jump      fn(t_name,*root,cb);
                    const motif           m( regexp::compile(*t_expr,mydb) );
                    const action          a(fn);
                    on(t_jump,m,a);
                }
                
                
                //! create a call
                /**
                 - to scanner 'name'
                 - when found 'expr'
                 - and performing callback host.meth(word)
                 */
                template <
                typename IDENTIFIER,
                typename EXPRESSION,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void call(const IDENTIFIER &name,
                                 const EXPRESSION &expr,
                                 OBJECT_POINTER    host,
                                 METHOD_POINTER    meth)
                {
                    check_root();
                    const tag    t_expr = tags::make( expr );
                    const tag    t_text = tags::make( flow::call::text );
                    const tag    t_name = tags::make( name );
                    const string temp   = *t_expr + *t_text + *t_name;
                    const tag    t_call = tags::make( temp );
                    
                    const flow::callback  cb(host,meth);
                    const flow::call      fn(t_name,*root,cb);
                    const motif           m( regexp::compile(*t_expr,mydb) );
                    const action          a(fn);
                    on(t_call,m,a);
                }
                
                //! create a return to calling scanner
                /**
                 - when found 'expr'
                 - and performing callback host.meth(word)
                 */
                template <
                typename EXPRESSION,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void back(const EXPRESSION &expr,
                                 OBJECT_POINTER    host,
                                 METHOD_POINTER    meth)
                {
                    check_root();
                    const tag    t_expr = tags::make( expr );
                    const tag    t_text = tags::make( flow::back::text );
                    const string temp   = *t_expr + *t_text;
                    const tag    t_back = tags::make( temp );
                    
                    const flow::callback  cb(host,meth);
                    const flow::back      fn(*root,cb);
                    const motif           m( regexp::compile(*t_expr,mydb) );
                    const action          a(fn);
                    on(t_back,m,a);
                }
                
                
                
                //______________________________________________________________
                //
                //! probe next lexeme
                /**
                 - find out the best motif
                 - if produce: emit lexeme
                 - if discard: drop lexeme, probe again
                 - if NULL:
                 -- if !ctrl : EOF
                 -- if  ctrl: something happened: should probe again (see analyzer)
                 */
                //______________________________________________________________
                lexeme  *probe(source &source,
                               bool   &ctrl);


                //______________________________________________________________
                //
                // other methods
                //______________________________________________________________
                void     link_to(analyzer&) throw();                   //!< set dict, root and indx
                void     restore(token&)    throw();                   //!< restore read token to root
                lexeme  *newlex(const tag &, const context &);         //!< create with *indx
                bool     standalone() const throw();                   //!< indx == &indx_
                bool     linked_to(const scanner &host) const throw(); //!< indx == &host.indx

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag                             label; //!< scanner label
                const auto_ptr<const instructions>    instr; //!< set of instructions
                const auto_ptr<const scatter::table>  table; //!< scattered directives
                const eos_policy                      onEOS; //!< to check EOS

            protected:
                source                                *flux; //!< set during a probe call
                analyzer                              *root; //!< set by link_to
                size_t                                *indx; //!< external/internal index
                const dictionary                      *mydb; //!< set by link_to

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(scanner);
                void check_word(const tag &uuid, const token &word) const;
                void check_root() const;
                size_t indx_; //!< default internal index for standalone scanner

            };
        }

    }
}

#endif

