//! \file

#ifndef YACK_UTF8_INCLUDED
#define YACK_UTF8_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
 
    struct utf8
    {
        static const uint32_t max_code = 0x010FFFF;
        enum status
        {
            header1,
            header2,
            header3,
            header4,
            headerB
        };
        
        static status query(uint8_t &data, uint8_t &bits, const uint8_t u);
        
        struct role
        {
            status  flag;
            uint8_t data;
            uint8_t bits;
        };
      
        static size_t encode(uint8_t data[], uint32_t code);
        
    };
    
}

#endif

