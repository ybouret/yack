//! \file

#ifndef YACK_JIVE_PATTERN_POSIX_INCLUDED
#define YACK_JIVE_PATTERN_POSIX_INCLUDED 1

#include "yack/jive/pattern.hpp"

namespace yack
{
    namespace jive
    {
        //----------------------------------------------------------------------
        //
        //! (extended) posix patterns
        //
        //----------------------------------------------------------------------
        struct posix
        {
            //------------------------------------------------------------------
            //
            // standard
            //
            //------------------------------------------------------------------
            static pattern *lower();  //!< \verbatim[a-z]\endverbatim
            static pattern *upper();  //!< \verbatim[A-Z]\endverbatim
            static pattern *alpha();  //!< \verbatim[a-zA-Z]\endverbatim
            static pattern *digit();  //!< \verbatim[0-9]\endverbatim
            static pattern *alnum();  //!< \verbatim[a-zA-Z0-9]\endverbatim
            static pattern *xdigit(); //!< \verbatim[0-9a-fA-F]\endverbatim
            static pattern *blank();  //!< \verbatim[ \t]\endverbatim
            static pattern *space();  //!< \verbatim[ \n\r\t\v\f]\endverbatim
            static pattern *punct();  //!< \verbatim][!"#$%&'()*+,./:;<=>?@\^_`{|}~-\endverbatim

            //------------------------------------------------------------------
            //
            // extended
            //
            //------------------------------------------------------------------
            static pattern *word();      //!< \verbatim[a-zA-Z0-9_]\endverbatim
            static pattern *endl();      //!< \verbatim\n|\r|\r\n\endverbatim
            static pattern *dot();       //!< \verbatim^endl\endverbatim
            static pattern *core();      //!< core 7-bits string content, minus quote, dquote  and backslash
            static pattern *vowel();     //!< vowels
            static pattern *consonant(); //!< consonants

            //------------------------------------------------------------------
            //
            // query
            //
            //------------------------------------------------------------------
            static pattern *create(const string &);

        };

    }

}

#endif
