
//! \file

#ifndef YACK_CHEMICAL_DESIGNER_COM_INCLUDED
#define YACK_CHEMICAL_DESIGNER_COM_INCLUDED 1


#include "yack/setup.hpp"

namespace yack
{
    namespace chemical
    {
        namespace nucleus
        {
            //__________________________________________________________________
            //
            //
            //! common identifiers
            //
            //__________________________________________________________________
            struct COM
            {
                static const char * const SPECIES;     //!< "SPECIES"
                static const char * const SP_NAME;     //!< "SP_NAME"
                static const char * const SP_EXPR;     //!< regular expression

                static const char * const COMPONENTS; //!< "COMPONENTS"
                static const char * const COEF;       //!< "COEF"
                static const char * const FIRST_COEF; //!< "FIRST_COEF"
                static const char * const EXTRA_COEF; //!< "EXTRA_COEF"

            };

        }

    }

}

#endif
