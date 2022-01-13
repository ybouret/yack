
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

                //! how to declare a new scanner
                enum decl_t
                {
                    declare_manual, //!< declare as manual => link_to
                    declare_plugin  //!< declare as plugin => already linked_to
                };


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

                //! declare a new scanner, post-process according to decl_t
                /**
                 - if declare_manual, scanner is linked to this
                 - if declare_plugin, plugin is already linked
                 */
                template <typename SCANNER>
                SCANNER & decl(SCANNER *s, const decl_t d = declare_manual )
                {
                    declare(s,d);
                    return *s;
                }

                //! register a plugin
                const plugin & import(plugin *);

                //! import a no-arg plugin
                template <
                typename PLUGIN,
                typename IDENTIFIER
                >
                inline const plugin & plug( type2type<PLUGIN>, const IDENTIFIER &uuid)
                {
                    return import( new PLUGIN(uuid,*this) );
                }

                //! import a 1-arg plugin
                template <
                typename PLUGIN,
                typename IDENTIFIER,
                typename EXPRESSION
                >
                inline const plugin & plug( type2type<PLUGIN>, const IDENTIFIER &uuid, const EXPRESSION &expr)
                {
                    return import( new PLUGIN(uuid,expr,*this) );
                }

                //! import a 2-args plugin
                template <
                typename PLUGIN,
                typename IDENTIFIER,
                typename EXPRESSINI,
                typename EXPRESSEND
                >
                inline const plugin & plug( type2type<PLUGIN>, const IDENTIFIER &uuid, const EXPRESSINI &init, const EXPRESSEND &done)
                {
                    return import( new PLUGIN(uuid,init,done,*this) );
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
                lexeme *      query(source &);         //!< query next lexeme
                void          store(lexeme *) throw(); //!< store a parsed lexeme
                void          reset()         throw(); //!< reset all

                //______________________________________________________________
                //
                // check status
                //______________________________________________________________
                const lexeme *peek(source &);                     //!< found next?
                bool          owns(const lexeme *) const throw(); //!< in repository ?

                //______________________________________________________________
                //
                // helper
                //______________________________________________________________
                void show(const char *pfx=NULL) const; //!< display what's in repository

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(analyzer);
                scanner *scan; //!< current scanner
                lexemes  repo; //!< cache
                history  hist; //!< call stack
                scan_set sdb;  //!< scanner database
                
                void     setup();
                void     declare(scanner *, const decl_t);
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

