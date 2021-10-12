#include "yack/apex/natural.hpp"
#include "yack/apex/alloc.hpp"
#include "yack/arith/align.hpp"
#include "yack/arith/base2.hpp"

#include <iostream>

namespace yack
{

    namespace apex
    {

        size_t natural:: size() const throw()
        {
            return bytes;
        }


        natural:: ~natural() throw()
        {
            bytes=0;
            words=0;
            alloc::field_release(word,coerce(words_exp2), coerce(block_exp2));
            coerce(words_size) = 0;
        }

#define YACK_APEX_NATURAL(WORDS_EXP2)                                              \
words_exp2(WORDS_EXP2),                                                            \
block_exp2(-1),                                                                    \
word( alloc::field_acquire<word_type>( coerce(words_exp2), coerce(block_exp2) ) ), \
words_size(size_t(1)<<words_exp2), block_size( size_t(1) << block_exp2 )


        natural:: natural(unsigned_type u) : number(), bytes(0), words(0), YACK_APEX_NATURAL(min_words_exp2)
        {
            static const size_t nw = sizeof(unsigned_type)/sizeof(word_type);

            assert(block_size>=sizeof(u));
            assert(words_size>=nw);
            
            word[0] = word_type(u);
            for(size_t i=1;i<nw;++i)
            {
                u >>= word_bits;
                word[i] = word_type(u);
            }

            words = nw;
            bytes = words << word_exp2;
            update();
        }

        natural:: natural(const natural &other) :
        number(),
        bytes(other.bytes),
        words(other.words),
        YACK_APEX_NATURAL(other.words_exp2)
        {
            for(size_t i=0;i<words;++i) word[i] = other.word[i];
        }

#define YACK_NATURAL_XCH(FIELD) coerce_cswap(FIELD,other.FIELD)

        void natural:: xch(natural &other) throw()
        {
            YACK_NATURAL_XCH(bytes);
            YACK_NATURAL_XCH(words);
            YACK_NATURAL_XCH(words_exp2);
            YACK_NATURAL_XCH(block_exp2);
            YACK_NATURAL_XCH(word);
            YACK_NATURAL_XCH(words_size);
            YACK_NATURAL_XCH(block_size);
        }

        natural & natural:: operator=(const natural &other)
        {
            if(this!=&other)
            {
                if(other.words<=words_size)
                {
                    words=other.words;
                    bytes=other.bytes;
                    for(size_t i=0;i<words;++i)          word[i] = other.word[i];
                    for(size_t i=words;i<words_size;++i) word[i] = 0;
                }
                else
                {
                    natural tmp(other);
                    xch(tmp);
                }
            }
            return *this;
        }


        void natural:: update() throw()
        {
            size_t msi = words-1;
            while(words>0&&word[msi]<=0)
            {
                --words;
                --msi;
            }


            if(words>0)
            {
                const  word_type msw = word[msi]; assert(msw>0);
                bytes = ( msi << word_exp2 ) + bytes_for(msw);
            }
            else
            {
                bytes = 0;
            }
        }

        size_t natural:: bits() const throw()
        {
            if(words>0)
            {
                const size_t    msi = words-1;
                const word_type msw = word[msi];
                return (msi * word_bits) + bits_for(msw);
            }
            else
            {
                return 0;
            }
        }

        unsigned_type natural:: lsu() const throw()
        {
            unsigned_type u = 0;
            for(size_t i=words;i>0;)
            {
                u <<= word_bits;
                u |=  word[--i];
            }
            return u;
        }

    }

}
