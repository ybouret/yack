
//! \file

#ifndef YACK_UTF_8_INCLUDED
#define YACK_UTF_8_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! UTF-8 encoding/decoding
    //
    //__________________________________________________________________________
    class UTF8
    {
    public:
        static  const char clid[]; //!< "UTF-8";
        typedef uint32_t   code_t;
        typedef uint8_t    byte_t;
        static const code_t code_mask = 0x001fffff;
        static const code_t info_mask = 0xff000000;
        static const code_t info_move = 24;

        template <typename T> struct range {
            T lower;
            T upper;
            inline bool owns(const T value) const throw()
            {
                return value>=lower && value<=upper;
            }
            inline size_t width() const throw() {
                return 1+size_t(upper)-size_t(lower);
            }
        };

        static const size_t        num_banks = 4;
        static const range<code_t> code_bank[num_banks];
        static const range<byte_t> byte_bank[num_banks];
        static const range<byte_t> continuation;
        
        static size_t validate(code_t &codepoint);
        
        UTF8(const code_t);
        UTF8(const UTF8&) throw();
        UTF8 & operator=(const UTF8 &) throw();
        UTF8 & operator=(code_t);
        ~UTF8() throw();
        
        size_t bytes()     const throw(); //! [1:4]
        code_t operator*() const throw();
        
    private:
        code_t code;
        
        
    };

}

#endif
