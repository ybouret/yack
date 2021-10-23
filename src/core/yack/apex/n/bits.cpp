
#include "yack/apex/natural.hpp"
#include "yack/system/endian.hpp"
#include "yack/arith/align.hpp"

#include <iostream>

namespace yack
{
    namespace apex
    {
        
        static const size_t _bit[8] =
        {
            size_t(1) << 0,
            size_t(1) << 1,
            size_t(1) << 2,
            size_t(1) << 3,
            size_t(1) << 4,
            size_t(1) << 5,
            size_t(1) << 6,
            size_t(1) << 7
        };
        
        bool natural::bit(const size_t ibit) const throw()
        {
            
            assert(ibit<bits());
            size_t        iB = ibit>>3;        assert(iB<bytes);
            const size_t  ib = ibit - (iB<<3); assert(ib<8);
            const uint8_t B  = (*this)[++iB];
            return 0 != (B & _bit[ib]);
        }
        
        
        
        
        namespace
        {
            typedef   uint8_t & (*apn_get_proc)(natural::word_type &, const size_t);
            
            static inline   uint8_t & getBE(natural::word_type &w, const size_t at) throw()
            {
                assert(at<sizeof(w));
                uint8_t *p= (uint8_t *)&w;
                return p[(sizeof(w)-1)-at];
            }
            
            static inline   uint8_t & getLE(natural::word_type &w, const size_t at) throw()
            {
                assert(at<sizeof(w));
                uint8_t *p = (uint8_t *)&w;
                return p[at];
            }
        }
        
        
        
        natural natural:: exp2(const size_t shift)
        {
            static const apn_get_proc proc =  endianness::BE() ? getBE : getLE;
            
            const size_t bits  = shift+1;
            const size_t bytes = YACK_ALIGN_ON(8,bits)>>3;
            const size_t words = YACK_ALIGN_ON(word_bits,bits)>>ilog2<word_bits>::value;
            const size_t ibyte = bytes-1;
            const size_t iword = words-1;
            const size_t ibit  = shift - (ibyte<<3); assert(ibit<8);
            
            
            natural res(words,as_capacity);
            
            res.bytes=bytes;
            res.words=words;
            
            proc(res.word[iword],ibyte-((ibyte>>word_exp2)<<word_exp2)) = _bit[ibit];
            
            return res;
            
        }
        
        
        natural & natural:: shr() throw()
        {
            if(words)
            {
                const size_t msi=words-1;
                for(size_t i=0,j=1;i<msi;++i,++j)
                {
                    word_type &w = word[i];
                    w >>= 1;
                    w |= ((word[j]&0x1) << (word_bits-1));
                }
                word[msi] >>= 1;
                update();
            }
            return *this;
        }
        
        void natural:: set_bit(const size_t ibit) throw()
        {
            size_t        iB = ibit>>3;
            const size_t  ib = ibit - (iB<<3); assert(ib<8);
            coerce((*this)[++iB]) |= _bit[ib];
        }
        
        
        
        natural natural:: shr(const size_t shift) const
        {
            
            if(shift<=0)
            {
                return *this;
            }
            else
            {
                const size_t my_bits = bits();
                if(shift>=my_bits)
                {
                    return natural(0);
                }
                else
                {
                    assert(shift<my_bits);
                    const size_t to_bits  = my_bits-shift;
                    const size_t to_words = YACK_ALIGN_ON(word_bits,to_bits)>>ilog2<word_bits>::value;
                    const size_t to_bytes = YACK_ALIGN_ON(8,to_bits)>>3;
                    
                    natural to(to_words,as_capacity);
                    to.words = to_words;
                    to.bytes = to_bytes;
                    for(size_t i=0,j=shift;i<to_bits;++i,++j)
                    {
                        if( bit(j) ) to.set_bit(i);
                    }
                    
                    
                    to.update();
                    return to;
                }
            }
        }
        
        natural natural:: shl(const size_t shift) const
        {
            if(shift<=0||words<=0)
            {
                return *this;
            }
            else
            {
                assert(words>0);
                const size_t my_bits  = bits(); assert(my_bits>0);
                const size_t to_bits  = my_bits+shift;
                const size_t to_words = YACK_ALIGN_ON(word_bits,to_bits)>>ilog2<word_bits>::value;
                const size_t to_bytes = YACK_ALIGN_ON(8,to_bits)>>3;
                
                natural to(to_words,as_capacity);
                to.words = to_words;
                to.bytes = to_bytes;
                
                for(size_t i=0,j=shift;i<my_bits;++i,++j)
                {
                    if( bit(i) ) to.set_bit(j);
                }
                
                to.update();
                return to;
            }
        }
        
        natural operator<<(const natural &lhs, const size_t shift)
        {
            return lhs.shl(shift);
        }
        
        natural operator>>(const natural &lhs, const size_t shift)
        {
            return lhs.shr(shift);
        }
        
        natural & natural:: operator<<=(const size_t shift)
        {
            natural res = shl(shift);
            xch(res);
            return *this;
        }
        
        natural & natural:: operator>>=(const size_t shift)
        {
            natural res = shr(shift);
            xch(res);
            return *this;
        }
    }
    
}

namespace yack
{
    namespace apex
    {
        natural natural::bitwise(bitproc proc, const natural &lhs, const natural &rhs)
        {
            assert(proc);
            const natural *lit = &lhs;
            const natural *big = &rhs;
            if(lit->bytes>big->bytes)
            {
                cswap(lit,big);
            }
            
            natural res(big->words,as_capacity);
            res.words = big->words;
            res.bytes = big->bytes;
            
            for(size_t i=lit->bytes;i>0;--i)
            {
                coerce( res[i] ) = proc( (*big)[i], (*lit)[i] );
            }
            
            for(size_t i=big->bytes;i>lit->bytes;--i)
            {
                coerce( res[i] ) = proc( (*big)[i], 0);
            }
            
            res.update();
            return res;
        }
        
        static uint8_t apn_or(const uint8_t x, const uint8_t y) throw()
        {
            return x|y;
        }
        
        static uint8_t apn_and(const uint8_t x, const uint8_t y) throw()
        {
            return x&y;
        }
        
        static uint8_t apn_xor(const uint8_t x, const uint8_t y) throw()
        {
            return x^y;
        }
        
        
#define YACK_APN_BITWISE_IMPL(OP,CODE)                                 \
natural operator OP (const natural  &lhs, const natural  &rhs) \
{ return natural::bitwise(CODE,lhs,rhs); } \
natural operator OP (const natural  &lhs, const uint_type rhs) \
{ const natural _(rhs);  return natural::bitwise(CODE,lhs,_); } \
natural operator OP (const uint_type lhs, const natural  &rhs) \
{ const natural _(lhs);  return natural::bitwise(CODE,_,rhs); } \
natural      & natural:: operator OP##=(const natural  &rhs)\
{ natural res = (*this) OP rhs; xch(res); return *this; }\
natural      & natural:: operator OP##=(const uint_type rhs) \
{ natural res = (*this) OP rhs; xch(res); return *this; }
        
        
        YACK_APN_BITWISE_IMPL(|,apn_or)
        YACK_APN_BITWISE_IMPL(&,apn_and)
        YACK_APN_BITWISE_IMPL(^,apn_xor)

        natural natural:: operator~() const
        {
            natural res(*this);
            for(size_t i=res.bytes;i>0;--i)
            {
                coerce(res[i]) = ~(*this)[i];
            }
            res.update();
            return res;
        }

        
    }
    
}
