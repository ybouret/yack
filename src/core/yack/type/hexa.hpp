
//! \file

#ifndef YACK_TYPE_HEXA_INCLUDED
#define YACK_TYPE_HEXA_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    struct hexa
    {
        static int      convert(const int ch) throw();
        static uint8_t  up16(const uint8_t) throw();
        static uint8_t  lo16(const uint8_t) throw();
        static const char lowercase_char[16];
        static const char uppercase_char[16];
        static const char lowercase_text[256][4];
        static const char uppercase_text[256][4];

    };

}

#endif
