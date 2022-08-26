//! \file

#ifndef YACK_INFO_COMPRESSION_ALPHABET_INCLUDED
#define YACK_INFO_COMPRESSION_ALPHABET_INCLUDED 1

#include "yack/data/list/raw.hpp"
#include "yack/arith/align.hpp"

namespace yack
{
    namespace information
    {

        namespace codec
        {

            template <typename CODE_TYPE>
            class glyph
            {
            public:
                typedef CODE_TYPE   code_type;
                static const size_t code_size = sizeof(code_type);
                static const size_t code_bits = code_size * 8 ;

                inline  glyph(const code_type ch) throw() : code(ch),      bits(code_bits) {}
                inline  glyph(const glyph    &gl) throw() : code(gl.code), bits(gl.bits) {}
                inline ~glyph()                   throw() { code=0; bits=0; }

                CODE_TYPE code;
                size_t    bits;

            private:
                YACK_DISABLE_ASSIGN(glyph);
            };


            template <typename CODE_TYPE>
            class symbol
            {
            public:
                typedef glyph<CODE_TYPE>               glyph_type;
                typedef typename glyph_type::code_type code_type;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(symbol);
            };

            

        }



    }
}

#endif
