//! \file

#ifndef YACK_CSV_INCLUDED
#define YACK_CSV_INCLUDED

#include "yack/csv/document.hpp"
#include "yack/jive/parser.hpp"
#include "yack/jive/syntax/translator.hpp"

namespace yack
{

    namespace CSV
    {

        //______________________________________________________________________
        //
        //
        //! parser of CSV documents
        //
        //______________________________________________________________________
        class Parser : public jive::parser
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Parser();          //!< setup
            virtual ~Parser() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //! produce a new document from jive::module
            //__________________________________________________________________
            Document * operator()(jive::module *m);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Parser);
            class Translator;
            Translator *tr;
        };
    }

}

#endif


