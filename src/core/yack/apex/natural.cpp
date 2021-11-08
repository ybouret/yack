#include "yack/apex/natural.hpp"
#include "yack/apex/m/archon.hpp"
#include "yack/arith/align.hpp"
#include "yack/arith/base2.hpp"
#include "yack/type/out-of-reach.hpp"
#include <cstring>

namespace yack
{

    namespace apex
    {



        size_t natural:: words_exp2_for(const size_t nw)
        {
            size_t wexp2 = min_words_exp2;
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
            YACK_APN_CHECK(*this,"~natural");
            bytes=0;
            words=0;
            archon::release_field(word,coerce(max_words_exp2), coerce(max_bytes_exp2));
            coerce(max_words) = 0;
        }

#define YACK_APEX_NATURAL(WORDS_EXP2)                                                      \
max_words_exp2(WORDS_EXP2),                                                                \
max_bytes_exp2(0),                                                                        \
word( archon::acquire_field<word_type>( coerce(max_words_exp2), coerce(max_bytes_exp2) ) ), \
max_words(size_t(1)<<max_words_exp2),                                                      \
max_bytes(size_t(1)<<max_bytes_exp2 )

 
        natural:: natural() :
        number(),
        readable<uint8_t>(),
        bytes(0),
        words(0),
        YACK_APEX_NATURAL(min_words_exp2)
        {
            YACK_APN_CHECK(*this,"natural()");
        }
        
        natural:: natural(uint_type u) :
        number(),
        readable<uint8_t>(),
        bytes(0),
        words(0),
        YACK_APEX_NATURAL(min_words_exp2)
        {
            ldu(u);
            YACK_APN_CHECK(*this,"natural(u)");
        }


        natural:: natural(const size_t num_words, const as_capacity_t &) :
        number(), readable<uint8_t>(),
        bytes(0),
        words(0),
        YACK_APEX_NATURAL(words_exp2_for(num_words))
        {
            assert(max_words>=num_words);
            YACK_APN_CHECK(*this,"natural(num_words,as_capacity)");
        }

        natural:: natural(const word_type *w, const size_t num_words) :
        number(), readable<uint8_t>(),
        bytes(num_words<<word_exp2),
        words(num_words),
        YACK_APEX_NATURAL(words_exp2_for(num_words))
        {
            assert(max_words>=num_words);
            memcpy(word,w,bytes);
            update();
            YACK_APN_CHECK(*this,"natural(w,n)");
        }

        void natural:: ldz() throw()
        {
            words = bytes = 0;
            memset(word,0,max_bytes);
            YACK_APN_CHECK(*this,"ldz");
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
        collection(),
        number(),
        readable<uint8_t>(),
        bytes(other.bytes),
        words(other.words),
        YACK_APEX_NATURAL(other.max_words_exp2)
        {
            for(size_t i=0;i<words;++i) word[i] = other.word[i];
            YACK_APN_CHECK(*this,"natural(other)");
        }

#define YACK_NATURAL_XCH(FIELD) coerce_cswap(FIELD,other.FIELD)

        void natural:: xch(natural &other) throw()
        {
            YACK_APN_CHECK(*this,"pre-xch");
            YACK_NATURAL_XCH(bytes);
            YACK_NATURAL_XCH(words);
            YACK_NATURAL_XCH(max_words_exp2);
            YACK_NATURAL_XCH(max_bytes_exp2);
            YACK_NATURAL_XCH(word);
            YACK_NATURAL_XCH(max_words);
            YACK_NATURAL_XCH(max_bytes);
            YACK_APN_CHECK(*this,"post-xch");
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
                    assert(*this==other);
                    YACK_APN_CHECK(*this,"operator=other");
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
            YACK_APN_CHECK(*this,"operator=u");
            return *this;
        }


        void natural:: zpad() throw()
        {
            for(size_t i=words;i<max_words;++i) word[i] = 0;
        }

        void natural:: update() throw()
        {
            //------------------------------------------------------------------
            //
            // check most significant index from words
            //
            //------------------------------------------------------------------
            size_t msi = words-1;
            while(words>0&&word[msi]<=0)
            {
                --words;
                --msi;
            }

            //------------------------------------------------------------------
            //
            // update bytes count
            //
            //------------------------------------------------------------------
            if(words>0)
            {
                const  word_type msw = word[msi]; assert(msw>0);
                bytes = ( msi << word_exp2 ) + bytes_for(msw);
            }
            else
            {
                bytes = 0;
            }

            //------------------------------------------------------------------
            //
            // clean remaining
            //
            //------------------------------------------------------------------
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

        natural natural:: factorial(uint_type n)
        {
            natural f = 1;
            while(n>1)
            {
                f *= n;
                --n;
            }
            return f;
        }

        
#define YACK_APN_CHECK_(COND) \
/**/  do { if( !(COND) )\
/**/       { std::cerr << "[apn] '" << #COND << "' FAILURE in <" << when << ">" << std::endl;\
/**/         exit(1);}\
/**/  } while(false)
        
        void natural:: check(const natural &self, const char *when)
        {
            //std::cerr << "<chk " << when << ">" << std::endl;
            assert(when);
            YACK_APN_CHECK_(NULL!=self.word);
            YACK_APN_CHECK_(self.words<=self.max_words);
            YACK_APN_CHECK_(self.bytes<=self.max_bytes);
            if(self.words>0)
            {
                const word_type msw = self.word[self.words-1];
                YACK_APN_CHECK_(msw!=0);
                YACK_APN_CHECK_( (self.words-1)*sizeof(word_type)+bytes_for(msw) == self.bytes);
            }
            for(size_t i=self.words;i<self.max_words;++i)
            {
                YACK_APN_CHECK_(0==self.word[i]);
            }
        }

        
    }

}



#include "yack/randomized/bits.hpp"


namespace yack
{

    namespace apex
    {

        natural:: natural(randomized::bits &ran, const size_t nbit) :
        number(),
        readable<uint8_t>(),
        bytes(YACK_ALIGN_ON(8,nbit) >> 3),
        words(YACK_ALIGN_TO(word_type,bytes)>>word_exp2),
        YACK_APEX_NATURAL(words_exp2_for(words))
        {
            if(bytes>0)
            {
                natural     &self = *this;
                const size_t top  = bytes-1;
                for(size_t i=top;i>0;--i)
                {
                    coerce(self[i]) = ran.to<uint8_t>();
                }
                const size_t xbit = nbit-(top<<3); assert(xbit>=1); assert(xbit<=8);
                uint8_t     &b    = coerce(self[bytes]);
                b = 1;
                for(size_t i=xbit;i>1;--i)
                {
                    b <<= 1;
                    if(ran.choice()) b |= 1;
                }
            }
            assert(nbit==bits());
            YACK_APN_CHECK(*this,"naural(ran,nbit)");
        }


    }

}

#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{

    namespace apex
    {
        void natural:: cast_overflow(const char *who) const
        {
            throw libc::exception(ERANGE,"apn::cast_to('%s')", (who?who:yack_unknown) );
        }
    }

}
