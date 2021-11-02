
//! \file

#ifndef YACK_TYPE_HEXA_INCLUDED
#define YACK_TYPE_HEXA_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! hexadecimal utilities
    //
    //__________________________________________________________________________
    struct hexa
    {
        static int        convert(const int ch) throw(); //!< 0-15 or -1 on error
        static uint8_t    up16(const uint8_t)   throw(); //!< (b>>4) &0xf
        static uint8_t    lo16(const uint8_t)   throw(); //!< b&0xf
        static const char lowercase_char[16];            //!< 0..9,a-f
        static const char uppercase_char[16];            //!< 0..9,A-F
        static const char lowercase_text[256][4];        //!< "00".."ff"
        static const char uppercase_text[256][4];        //!< "00".."FF"

    };

}

#endif
