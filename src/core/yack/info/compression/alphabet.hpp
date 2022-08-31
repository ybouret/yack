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
            enum escape_glyph
            {
                escape_nyt=0,
                escape_end=1
            };
            
            typedef int2type<escape_nyt> ESC_NYT_t;
            extern const ESC_NYT_t       ESC_NYT;

            typedef int2type<escape_end> ESC_END_t;
            extern const ESC_END_t       ESC_END;

            struct glyph_ops
            {
                static void display_raw(std::ostream &os, const void *addr, const size_t size);
            };


            template <const size_t CODE_BITS>
            class glyph
            {
            public:
                static  const    size_t    code_bits = CODE_BITS;
                static  const    size_t    code_size = CODE_BITS/8;
                static  const    size_t    word_size = code_size*2;
                static  const    size_t    word_bits = word_size*8;
                typedef typename unsigned_int<code_size>::type code_type;
                typedef typename unsigned_int<word_size>::type word_type;
                static  const    word_type cntl_mini = (1<<code_bits);
                static  const    size_t    cntl_bits = code_bits+1;
                static  const    word_type cntl_maxi = (1<<cntl_bits)-1;


                inline  glyph(const code_type c) throw() : code(c), bits(code_bits) {}
                inline ~glyph() throw() { code=0; bits=0; }
                inline  glyph(const glyph &g) throw() : code(g.code), bits(g.bits) {}
                inline  glyph & operator=(const glyph &g) throw() { code=g.code; bits=g.bits; return *this; }

                template <int ESC>
                inline glyph(const int2type<ESC> &_) throw() : code(cntl_mini+_.value), bits(cntl_bits)
                {
                }

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



                word_type code;
                size_t    bits;
                



            };



            

        }



    }
}

#endif
