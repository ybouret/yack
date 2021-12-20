
//! \file

#ifndef YACK_JIVE_LEXICAL_ANALYZER_INCLUDED
#define YACK_JIVE_LEXICAL_ANALYZER_INCLUDED 1

#include "yack/jive/lexical/plugin.hpp"
#include "yack/jive/pattern/dictionary.hpp"
#include "yack/pipe/stack.hpp"
#include "yack/sequence/list.hpp"


namespace yack
{
    namespace jive
    {

        namespace lexical
        {
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
                typedef stack<scanner *,list<scanner*> > history;  //!< call stack



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
                // setup methods
                //______________________________________________________________
                const scan_set & operator*() const throw(); //!< access to db

                //! declare and link a new scanner
                template <typename SCANNER>
                SCANNER & decl( SCANNER *s )
                {
                    declare(s);
                    return *s;
                }

                void plug( plugin *plg )
                {
                    assert(NULL!=plg);
                    plugin &p = decl<plugin>(plg);
                    call(p.label,p.trigger,&p,&plugin::enter);
                }
                

                //! access existing scanner by label
                const scanner & operator[](const string &label) const;
                

                //______________________________________________________________
                //
                // flow methods
                //______________________________________________________________
                void flow_jump(const string &target); //!< change scanner
                void flow_call(const string &target); //!< call scanner
                void flow_back();                     //!< back from calling scanner
                

                //______________________________________________________________
                //
                // high-level analyzing
                //______________________________________________________________
                lexeme * query(source &);         //!< query next lexeme
                void     store(lexeme *) throw(); //!< store a parsed lexeme

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(analyzer);
                scanner *scan; //!< current scanner
                lexemes  repo; //!< cache
                history  hist; //!< call stack
                scan_set sdb;
                void     setup();
                void     declare(scanner *);
                scanner *request(const string &target, const char *when) const;
                
                
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

