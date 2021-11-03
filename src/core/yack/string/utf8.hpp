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
    class utf8
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        static const char clid[]; //!< "UTF-8"
        typedef  uint32_t type;
      
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        utf8() throw();
        utf8(const type codepoint);
        utf8(const utf8 &) throw();
        utf8 & operator=( const utf8 &other ) throw();
        utf8 & operator=( type codepoint);
      
        void xch(utf8 &) throw();
        
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
        type    code;
        size_t  clen;
        
        static size_t validate( type &codepoint);
        struct bank {
            type lower;
            type upper;
            bool owns(const type) const throw();
        };
        static const bank banks[4];
    };
    
}

#endif

