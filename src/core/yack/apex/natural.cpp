#include "yack/apex/natural.hpp"
#include "yack/apex/n/alloc.hpp"
#include "yack/arith/align.hpp"
#include "yack/arith/base2.hpp"
#include "yack/type/out-of-reach.hpp"
#include <cstring>

namespace yack
{

    namespace apex
    {

        int natural:: words_exp2_for(const size_t nw)
        {
            int    wexp2 = min_words_exp2;
            size_t wsize = min_words_size;
            while(wsize<nw)
            {
                wsize <<= 1;
                ++wexp2;
            }
            return wexp2;
        }

        size_t natural:: size() const throw()
        {
            return bytes;
        }


        natural:: ~natural() throw()
        {
            bytes=0;
            words=0;
            alloc::field_release(word,coerce(max_words_exp2), coerce(max_bytes_exp2));
            coerce(max_words) = 0;
        }

#define YACK_APEX_NATURAL(WORDS_EXP2)                                                      \
max_words_exp2(WORDS_EXP2),                                                                \
max_bytes_exp2(-1),                                                                        \
word( alloc::field_acquire<word_type>( coerce(max_words_exp2), coerce(max_bytes_exp2) ) ), \
max_words(size_t(1)<<max_words_exp2),                                                      \
max_bytes(size_t(1)<<max_bytes_exp2 )


        natural:: natural(uint_type u) :
        number(),
        readable<uint8_t>(),
        words(0), bytes(0), YACK_APEX_NATURAL(min_words_exp2)
        {
            ldu(u);
        }


        natural:: natural(const size_t num_words, const as_capacity_t &) :
        number(), readable<uint8_t>(),
        words(0), bytes(0),YACK_APEX_NATURAL(words_exp2_for(num_words))
        {
            assert(max_words>=num_words);

        }

        natural:: natural(const word_type *w, const size_t num_words) :
        number(), readable<uint8_t>(),
        words(0), bytes(0),YACK_APEX_NATURAL(words_exp2_for(num_words))
        {
            assert(max_words>=num_words);
            memcpy(word,w,( bytes = (words=num_words) << word_exp2) );
            update();
        }

        void natural:: ldz() throw()
        {
            words = bytes = 0;
            memset(word,0,max_bytes);
        }

        size_t natural:: ldw(word_type *w,
                             uint_type  u,
                             size_t    &n) throw()
        {
            // store words
            w[0] = word_type(u);
            for(size_t i=1;i<words_per_uint;++i)
            {
                u >>= word_bits;
                w[i] = word_type(u);
            }

            // compute number of significant words
            size_t num = words_per_uint;
            size_t msi = words_per_uint-1;
            while(num>0&&w[msi]<=0)
            {
                --num;
                --msi;
            }
            
            // compute bytes
            if(num>0)
            {
                assert(w[msi]>0);
                n = (msi << word_exp2) + bytes_for( w[msi] );
            }
            else
            {
                n=0;
            }
            
            return num;
        }

        const natural::word_type * natural:: u2w(uint_type &value,
                                                 size_t    &num_words,
                                                 size_t    &num_bytes) throw()
        {
            word_type temp[words_per_uint]; assert(sizeof(temp)==sizeof(uint_type));
            num_words = ldw(temp,value,num_bytes);
            return static_cast<word_type *>( out_of_reach::copy(&value,temp,sizeof(temp)) );
        }

        void natural:: ldu(uint_type u) throw()
        {
            assert(max_bytes>=sizeof(uint_type));
            assert(max_words>=words_per_uint);

            words = ldw(word,u,bytes);
            zpad();
        }


        natural:: natural(const natural &other) :
        number(),
        readable<uint8_t>(),
        words(other.words),
        bytes(other.bytes),
        YACK_APEX_NATURAL(other.max_words_exp2)
        {
            for(size_t i=0;i<words;++i) word[i] = other.word[i];
        }

#define YACK_NATURAL_XCH(FIELD) coerce_cswap(FIELD,other.FIELD)

        void natural:: xch(natural &other) throw()
        {
            YACK_NATURAL_XCH(bytes);
            YACK_NATURAL_XCH(words);
            YACK_NATURAL_XCH(max_words_exp2);
            YACK_NATURAL_XCH(max_bytes_exp2);
            YACK_NATURAL_XCH(word);
            YACK_NATURAL_XCH(max_words);
            YACK_NATURAL_XCH(max_bytes);
        }

        natural & natural:: operator=(const natural &other)
        {
            if(this!=&other)
            {
                if(other.words<=max_words)
                {
                    words=other.words;
                    bytes=other.bytes;
                    for(size_t i=0;i<words;++i) word[i] = other.word[i];
                    zpad();
                }
                else
                {
                    natural tmp(other);
                    xch(tmp);
                }
            }
            return *this;
        }

        natural & natural:: operator= (const uint_type u) throw()
        {
            ldu(u);
            return *this;
        }


        void natural:: zpad() throw()
        {
            for(size_t i=words;i<max_words;++i) word[i] = 0;
        }

        void natural:: update() throw()
        {
            // check most significant index from words
            size_t msi = words-1;
            while(words>0&&word[msi]<=0)
            {
                --words;
                --msi;
            }

            // updated bytes
            if(words>0)
            {
                const  word_type msw = word[msi]; assert(msw>0);
                bytes = ( msi << word_exp2 ) + bytes_for(msw);
            }
            else
            {
                bytes = 0;
            }

            // clean remaining
            zpad();
        }

        size_t natural:: bits() const throw()
        {
            if(words>0)
            {
                const size_t    msi = words-1;
                const word_type msw = word[msi]; assert(msw>0);
                return (msi * word_bits) + bits_for(msw);
            }
            else
            {
                return 0;
            }
        }

        uint_type natural:: lsu() const throw()
        {
            uint_type u = 0;
            for(size_t i=words;i>0;)
            {
                u <<= word_bits;
                u |=  word[--i];
            }
            return u;
        }




    }

}



