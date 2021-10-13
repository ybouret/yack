#include "yack/apex/natural.hpp"
#include "yack/apex/alloc.hpp"
#include "yack/arith/align.hpp"
#include "yack/arith/base2.hpp"
#include "yack/type/out-of-reach.hpp"
#include <cstring>

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
            alloc::field_release(word,coerce(max_words_exp2), coerce(max_bytes_exp2));
            coerce(max_words) = 0;
        }

#define YACK_APEX_NATURAL(WORDS_EXP2)                                                      \
max_words_exp2(WORDS_EXP2),                                                                \
max_bytes_exp2(-1),                                                                        \
word( alloc::field_acquire<word_type>( coerce(max_words_exp2), coerce(max_bytes_exp2) ) ), \
max_words(size_t(1)<<max_words_exp2),                                                      \
max_bytes(size_t(1)<<max_bytes_exp2 )


        natural:: natural(unsigned_type u) : number(), words(0), bytes(0), YACK_APEX_NATURAL(min_words_exp2)
        {
            ldu(u);
        }


        void natural:: ldz() throw()
        {
            words = bytes = 0;
            memset(word,0,max_bytes);
        }

        void natural:: ldu(unsigned_type u) throw()
        {
            static const size_t nw = sizeof(unsigned_type)/sizeof(word_type);

            assert(max_bytes>=sizeof(u));
            assert(max_words>=nw);

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
                    for(size_t i=0;i<words;++i)         word[i] = other.word[i];
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

        natural & natural:: operator= (const unsigned_type u) throw()
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

#include "yack/system/endian.hpp"

namespace yack
{
    namespace apex
    {
        namespace
        {
            typedef const uint8_t & (*apn_get_proc)(const natural::word_type &, const size_t);

            static inline const uint8_t & getBE(const natural::word_type &w, const size_t at) throw()
            {
                assert(at<sizeof(w));
                const uint8_t *p= (const uint8_t *)&w;
                return p[(sizeof(w)-1)-at];
            }

            static inline const uint8_t & getLE(const natural::word_type &w, const size_t at) throw()
            {
                assert(at<sizeof(w));
                const uint8_t *p= (const uint8_t *)&w;
                return p[at];
            }
        }


        const uint8_t & natural:: operator[](const size_t indx) const throw()
        {
            static const apn_get_proc proc =  endianness::BE() ? getBE : getLE;

            assert(indx>0);
            assert(indx<=bytes);

            const size_t ib = indx-1;
            const size_t iw = ib>>word_exp2;
            const size_t at = ib-(iw<<word_exp2);
            return proc(word[iw],at);
        }

    }

}

#include "yack/type/hexa.hpp"
#include <iostream>

namespace yack
{
    namespace apex
    {

        std::ostream & natural:: output_hex(std::ostream &os) const
        {
            if(bytes<=0)
                os << "0";
            else
            {
                // first byte
                {
                    const uint8_t b = (*this)[bytes]; assert(b>0);
                    {
                        const uint8_t up16 = hexa::up16(b);
                        if(up16>0) os << hexa::lowercase_char[up16];
                        /*      */ os << hexa::lowercase_char[hexa::lo16(b)];
                    }
                }

                // other bytes
                for(size_t i=bytes-1;i>0;--i)
                {
                    os << hexa::lowercase_text[ (*this)[i] ];
                }

            }
            return os;
        }

        std::ostream & operator<<(std::ostream &os, const natural n)
        {
            return n.output_hex(os);
        }


    }

}

