//! \file

#ifndef YACK_UTF8_INCLUDED
#define YACK_UTF8_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! UTF-8 encoding/decoding
    //
    //__________________________________________________________________________
    struct utf8
    {
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        static const char clid[]; //!< "UTF-8"
       
        //! decoding status
        enum decoding
        {
            decoding_done,  //!< done
            decoding_wait1, //!< wait for 1 byte
            decoding_wait2, //!< wait for 2 bytes
            decoding_wait3  //!< waut for 3 bytes
        };
        
        
        //! encode valid codepoint, return number of bytes
        static size_t   encode(uint8_t data[], uint32_t code);
        
        //! initialize decoding sequence
        static decoding decode_init(uint32_t &code, const uint8_t data);
        
        //! update code according to status
        static decoding decode_next(uint32_t &code, const uint8_t data, const decoding flag);
       
        //! decode a valid sequence
        static uint32_t decode(const uint8_t data[], const size_t size);
        
    private:
        static uint8_t  decode6bits(const uint8_t data);
        
    };
    
}

#endif

