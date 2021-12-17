
//! \file

#ifndef YACK_JIVE_LEXICAL_ANALYZER_INCLUDED
#define YACK_JIVE_LEXICAL_ANALYZER_INCLUDED 1

#include "yack/jive/lexical/scanner.hpp"
#include "yack/jive/pattern/dictionary.hpp"

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
                typedef ark_ptr<string,scanner>     scan_ptr; //!< alias
                typedef suffix_set<string,scan_ptr> scan_set; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup, release database and liberate self
                virtual ~analyzer() throw();

                //! setup, withhold and self registration
                template <typename LABEL> inline
                analyzer(const LABEL &id) : scanner(id,&dict), sdb(), dict()
                {
                    setup();
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const scan_set & operator*() const throw(); //!< access to db

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(analyzer);
                scan_set sdb;
                void     setup();

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

