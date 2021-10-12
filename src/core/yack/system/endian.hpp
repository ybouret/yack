
//! \file

#ifndef YACK_ENDIAN_INCLUDED
#define YACK_ENDIAN_INCLUDED 1

#include "yack/system/setup.h"

namespace yack
{
    struct endianness
    {
        static bool BE() throw();
        static bool LE() throw();
    };
}

#endif

