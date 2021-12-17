
//! \file

#ifndef YACK_JIVE_LEXICAL_ANALYZER_INCLUDED
#define YACK_JIVE_LEXICAL_ANALYZER_INCLUDED 1

#include "yack/jive/lexical/scanner.hpp"
#include "yack/jive/pattern/dictionary.hpp"
#include "yack/pipe/stack.hpp"
#include "yack/sequence/list.hpp"


namespace yack
{
    namespace jive
    {

        namespace lexical
        {


            typedef functor<void,TL1(token&)> callback;

            class regulator
            {
            public:
                const tag       name;
                const callback  proc;
                analyzer       &ctrl;

                virtual ~regulator() throw();

            protected:
                regulator(const tag      &,
                          const callback &,
                          analyzer       &);

                regulator(const regulator &);

            private:
                YACK_DISABLE_ASSIGN(regulator);
            };

            class jump : public regulator
            {
            public:
                explicit jump(const tag      &n,
                              const callback &p,
                              analyzer       &c) :
                regulator(n,p,c)
                {
                }

                virtual ~jump() throw()
                {

                }

                jump(const jump &_) : regulator(_) {}
                
                behavior operator()(token &word);


            private:
                YACK_DISABLE_ASSIGN(jump);
            };








            //__________________________________________________________________
            //
            //
            //! lexical analyzer with multiple (control) scanners
            //
            //__________________________________________________________________
            class analyzer : public scanner
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef ark_ptr<string,scanner>          scan_ptr; //!< alias
                typedef suffix_set<string,scan_ptr>      scan_set; //!< alias
                typedef stack<scanner *,list<scanner*> > history;



                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup, release database and liberate self
                virtual ~analyzer() throw();

                //! setup, withhold and self registration
                template <typename LABEL> inline
                analyzer(const LABEL &id) :
                scanner(id),
                scan(this),
                repo(),
                hist(),
                sdb(),
                dict()
                {
                    setup();
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const scan_set & operator*() const throw(); //!< access to db

                template <typename SCANNER>
                SCANNER & decl( SCANNER *s )
                {
                    declare(s);
                    return *s;
                }


                template <
                typename EXPRESSION,
                typename IDENTIFIER,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                > inline
                void jump(const EXPRESSION     &expr,
                          const IDENTIFIER     &whom,
                          OBJECT_POINTER        host,
                          METHOD_POINTER        meth)
                {

                    // create expr.jmp.whom
                    const tag    txpr = tags::make(expr);
                    const tag    tjmp = tags::make("@");
                    const tag    ttgt = tags::make(whom);
                    const string temp = *txpr + *tjmp + *ttgt;
                    const tag    name = tags::make(temp);

                    // create callback
                    const callback proc(host,meth);

                    // create regulator
                    const lexical::jump ctrl(ttgt,proc,*this);

                    // create motif
                    const motif          info( regexp::compile(*txpr,&dict) );

                    // create action
                    on(name,info,ctrl);
                }

                void do_jump(const string &);

                lexeme * query(source &);
                void     store(lexeme *) throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(analyzer);
                scanner *scan; //!< current scanner
                lexemes  repo; //!< cache
                history  hist; //!< call stack
                scan_set sdb;
                void     setup();
                void     declare(scanner *);

            public:
                dictionary dict; //!< shared dictionary

            };

        }

        //______________________________________________________________________
        //
        //
        //! a lexer is a lexical analyzer
        //
        //______________________________________________________________________
        typedef lexical::analyzer lexer;

    }

}

#endif

