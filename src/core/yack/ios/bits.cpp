
#include "yack/ios/bits.hpp"

namespace yack
{
    io_bits:: io_bits() throw() :
    io_list(),
    ios::ostream(),
    ios::istream(),
    pool()
    {
    }

    io_bits:: ~io_bits() throw()
    {
    }


    io_bit * io_bits:: fast(const io_bit::type value) const throw()
    {
        assert(pool.size>0);
        io_bit *b = pool.query();
        *b        = value;
        return b;
    }

    io_bit * io_bits:: make(const io_bit::type value)
    {
        return (pool.size>0) ? fast(value) : new io_bit( value );

    }

    io_bits :: io_bits(const io_bits &other) :
    io_list(),
    ios::ostream(),
    ios::istream(),
    pool()
    {
        const size_t n = other.size;
        const size_t p = other.pool.size;
        if(p>=n)
        {
            for(const io_bit *b = other.head; b; b=b->next)
            {
                push_back( other.fast(**b) );
            }
        }
        else
        {
            assert(p<n);
            const io_bit *b = other.head;
            for(size_t i=p;i>0;--i,b=b->next)
            {
                assert(NULL!=b);
                push_back( other.fast(**b) );
            }
            for(size_t i=n-p;i>0;--i,b=b->next)
            {
                assert(NULL!=b);
                push_back( new io_bit( **b) );
            }
        }
    }


    void io_bits:: add(const io_bit::type value)
    {
        push_back( make(value) );
    }

    void io_bits:: pre(const io_bit::type value)
    {
        push_front( make(value) );
    }


    static inline uint8_t read8(const io_bit * &curr) throw()
    {
        uint8_t    res = 0;
        for(size_t i=8;i>0;--i)
        {
            assert(curr);
            res <<= 1;
            if(**curr) res |= 1;
            curr=curr->next;
        }
        return res;
    }


    static inline uint8_t read_(const io_bit * &curr) throw()
    {
        uint8_t    res = 0;
        while(curr)
        {
            assert(curr);
            res <<= 1;
            if(**curr) res |= 1;
            curr=curr->next;
        }
        return res;
    }


    void io_bits:: release() throw()
    {
        while(size) pool.store( pop_back() );
    }

    void io_bits:: write(const char C)
    {
        append<char>(C);
    }

    const unsigned io_bits::npad[8] =
    {
        0, 7, 6, 5, 4, 3, 2, 1
    };

    void io_bits:: flush()
    {
        for(unsigned i=npad[size&7];i>0;--i) add(_0);
        assert(0==(size%8));
    }


    bool  io_bits::  query_(char &C) throw()
    {
        if(size>=8)
        {
            C = pluck<char>();
            return true;
        }
        else
        {
            return false;
        }
    }

    size_t io_bits:: fetch_(void *addr, const size_t size) throw()
    {
        assert(yack_good(addr,size));
        const size_t nc = size/8;
        const size_t res = min_of(nc,size);

        {
            uint8_t      *dest = static_cast<uint8_t *>(addr);
            const io_bit *curr = head;
            for(size_t i=res;i>0;--i)
            {
                *(dest++) = read8(curr);
            }
        }
        
        return res;
    }

    void io_bits:: drop() throw()
    {
        while(size&7) pool.store( pop_front() );
    }

}

#include "yack/randomized/bits.hpp"

namespace yack
{
    void io_bits:: fuzz(randomized::bits &ran)
    {
        for(unsigned i=npad[size&7];i>0;--i) add(ran.choice()?_1:_0);
        assert(0==(size%8));
    }
}

#include <iostream>
#include "yack/ios/ascii/hybrid.hpp"

namespace yack
{
    std::ostream & operator<<(std::ostream &os, const io_bits &Q)
    {


        size_t       nc = Q.size/8;
        const size_t nr = Q.size - nc*8;
        const io_bit *b  = Q.head;

        if(nc>0) {
            os << '[';
            while(nc-- > 0)
            {
                const uint8_t u = read8(b);
                os << ios::ascii::hybrid[u];
            }
            os << ']';
        }

        if(nr>0)
        {
            assert(NULL!=b);
            os << '[' << ios::ascii::hybrid[ read_(b) ] << ']';
        }
        return os;
    }

}

#include "yack/apex/natural.hpp"

namespace yack
{

    void io_bits::add(const apex::natural &n)
    {
        size_t i=n.bits();
        while(i-- > 0)
        {
            if(n.bit(i)) add(_1); else add(_0);
        }
    }

    apex::natural io_bits::ap(size_t nbit)
    {
        assert(nbit<=size);
        apex::natural n;
        while(nbit-- > 0)
        {
            n = n.shl(1);
            if( **pool.store(pop_front())  )
            {
                n |= 1;
            }
        }
        return n;
    }

}
