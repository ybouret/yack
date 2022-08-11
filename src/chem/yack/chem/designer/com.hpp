
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

            struct COM
            {
                static const char SPECIES[];     //!< "SPECIES"
                static const char SP_NAME[];     //!< "SP_NAME"
                static const char SP_EXPR[];     //!< regular expression

                static const char COMPONENTS[]; //!< "COMPONENTS"
                static const char COEF[];       //!< "COEF"
                static const char FIRST_COEF[]; //!< "FIRST_COEF"
                static const char EXTRA_COEF[]; //!< "EXTRA_COEF"

            };

        }

    }

}

#endif
