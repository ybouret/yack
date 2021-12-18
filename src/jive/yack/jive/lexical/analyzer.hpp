
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
                // methods
                //______________________________________________________________
                const scan_set & operator*() const throw(); //!< access to db

                //! declare and link a new scanner
                template <typename SCANNER>
                SCANNER & decl( SCANNER *s )
                {
                    declare(s);
                    return *s;
                }

                //! access existing scanner by lable
                const scanner & operator[](const string &label) const;
                
                
                void impl_jump(const string &target); //!< change scanner
                void impl_call(const string &target); //!< call scanner
                void impl_back();                     //!< back from calling scanner
                
               
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

