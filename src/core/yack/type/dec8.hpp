
//! \file

#ifndef YACK_TYPE_HEXA_INCLUDED
#define YACK_TYPE_HEXA_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! 8bits decimal
    //
    //__________________________________________________________________________
    struct dec8
    {
        static const char     text[256][4]; //!< "0".."255"
        static const uint8_t  size[256];    //!< 1...3
    };
}

#endif
