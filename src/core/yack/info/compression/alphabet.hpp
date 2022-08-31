//! \file

#ifndef YACK_INFO_COMPRESSION_ALPHABET_INCLUDED
#define YACK_INFO_COMPRESSION_ALPHABET_INCLUDED 1

#include "yack/data/list/raw.hpp"
#include "yack/type/ints.hpp"
#include <iostream>

namespace yack
{
    namespace information
    {

        namespace codec
        {
            //__________________________________________________________________
            //
            //
            //! escape sequence naming
            //
            //__________________________________________________________________
            enum escape_glyph
            {
                escape_nyt=0, //!< Not Yet Transmitted
                escape_end=1  //!< flushing
            };
            
            typedef int2type<escape_nyt> ESC_NYT_t; //!< alias
            extern const ESC_NYT_t       ESC_NYT;   //!< alias

            typedef int2type<escape_end> ESC_END_t; //!< alias
            extern const ESC_END_t       ESC_END;   //!< alias

            //__________________________________________________________________
            //
            //
            //! anonymous glyph operations
            //
            //__________________________________________________________________
            struct glyph_ops
            {
                //! display raw data
                static void display_raw(std::ostream &os, const void *addr, const size_t size);
            };

            //__________________________________________________________________
            //
            //
            //! generic glyph
            //
            //__________________________________________________________________
            template <const size_t CODE_BITS>
            class glyph
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static  const    size_t    code_bits = CODE_BITS;         //!< code bits
                static  const    size_t    code_size = CODE_BITS/8;       //!< code bytes
                static  const    size_t    word_size = code_size*2;       //!< word bytes, greater than code bytes
                static  const    size_t    word_bits = word_size*8;       //!< word bits
                typedef typename unsigned_int<code_size>::type code_type; //!< matching code type
                typedef typename unsigned_int<word_size>::type word_type; //!< matching word type
                static  const    word_type cntl_mini = (1<<code_bits);    //!< minimum control value
                static  const    size_t    cntl_bits = code_bits+1;       //!< bits for control, one more than code
                static  const    word_type cntl_maxi = (1<<cntl_bits)-1;  //!< maximum control value

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline  glyph(const code_type c)          throw() : code(c), bits(code_bits) {}               //!< setup
                inline ~glyph()                           throw() { code=0; bits=0; }                         //!< cleanup
                inline  glyph(const glyph &g)             throw() : code(g.code), bits(g.bits) {}             //!< copy
                inline  glyph & operator=(const glyph &g) throw() { code=g.code; bits=g.bits; return *this; } //!< assign

                //! assign an escape/control sequence
                template <int ESC>
                inline glyph(const int2type<ESC> &_) throw() : code(cntl_mini+_.value), bits(cntl_bits)
                {
                }


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! display (raw)
                inline friend std::ostream & operator<<(std::ostream &os, const glyph &g)
                {
                    if(g.bits<=code_bits)
                    {
                        glyph_ops::display_raw(os, & g.code, code_size );
                    }
                    else
                    {
                        os << "[ESC+" << int(g.code-cntl_mini) << "]";
                    }
                    return os;
                }


                //______________________________________________________________
                //
                // members
                //______________________________________________________________

                word_type code; //!< code on a word
                size_t    bits; //!< bits

            };



            

        }



    }
}

#endif
