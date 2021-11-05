
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
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        static  const char    clid[];                 //!< "UTF-8";
        static const uint32_t code_mask = 0x001fffff; //!< 21 bits
        static const uint32_t info_mask = 0xff000000; //!< to store info
        static const uint32_t info_move = 24;         //!< info shifting

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

        static const size_t          num_banks = 4;
        static const range<uint32_t> code_bank[num_banks];
        static const range<uint8_t>  byte_bank[num_banks];
        static const range<uint8_t>  continuation;

        //! decoding status
        enum decoding
        {
            decoding_done,  //!< done
            decoding_wait1, //!< wait for 1 byte
            decoding_wait2, //!< wait for 2 bytes
            decoding_wait3  //!< waut for 3 bytes
        };
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        UTF8(const uint32_t);
        UTF8(const UTF8&) throw();
        UTF8 & operator=(const UTF8 &) throw();
        UTF8 & operator=(uint32_t);
        ~UTF8() throw();




        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        static void   validate(uint32_t &codepoint);       //!< check/format codepoint
        void          xch(UTF8 &)                 throw(); //!< no-throw exchange
        size_t        bytes()               const throw(); //!< [1:4]
        uint32_t      operator*()           const throw(); //!< 21-bits of code
        void          encode(uint8_t *data) const throw(); //!< code to data[bytes]



#define YACK_UTF8_CMP(OP)\
/**/    inline friend bool operator OP (const UTF8 &lhs, const UTF8 &rhs) throw()\
/**/    { return compare(lhs,rhs) OP 0; }
#define YACK_UTF8_COMPARISONS() \
/**/    YACK_UTF8_CMP(==)       \
/**/    YACK_UTF8_CMP(!=)       \
/**/    YACK_UTF8_CMP(<=)       \
/**/    YACK_UTF8_CMP(>=)       \
/**/    YACK_UTF8_CMP(<)        \
/**/    YACK_UTF8_CMP(>)        \
/**/    static int compare(const UTF8 &lhs, const UTF8 &rhs) throw()

        YACK_UTF8_COMPARISONS();

        //______________________________________________________________________
        //
        // decoding methods
        //______________________________________________________________________
        //! initialize decoding sequence
        static decoding decode_init(uint32_t &code, const uint8_t data);

        //! update code according to status
        static decoding decode_next(uint32_t &code, const uint8_t data, const decoding flag);

        //! decode a valid sequence
        static UTF8 decode(const uint8_t data[], const size_t size);

    private:
        uint32_t code;
    };

}

#endif
