//! \file

#ifndef YACK_UTF8_INCLUDED
#define YACK_UTF8_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
 
    struct utf8
    {
        
        static size_t encode(uint8_t data[], uint32_t code);
        
    };
    
}

#endif

