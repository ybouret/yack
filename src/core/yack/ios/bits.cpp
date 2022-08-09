
#include "yack/ios/bits.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include <iostream>

namespace yack
{
    io_bits:: io_bits() throw() : io_list(), pool()
    {
    }

    io_bits:: ~io_bits() throw()
    {
    }


    io_bit * io_bits:: query_(const io_bit::type value) const throw()
    {
        assert(pool.size>0);
        io_bit *b = pool.query();
        *b        = value;
        return b;
    }

    io_bit * io_bits:: create(const io_bit::type value)
    {
        return (pool.size>0) ? query_(value) : new io_bit( value );

    }

    io_bits :: io_bits(const io_bits &other) : io_list(), pool()
    {
        const size_t n = other.size;
        const size_t p = other.pool.size;
        if(p>=n)
        {
            for(const io_bit *b = other.head; b; b=b->next)
            {
                push_back( other.query_(**b) );
            }
        }
        else
        {
            assert(p<n);
            const io_bit *b = other.head;
            for(size_t i=p;i>0;--i,b=b->next)
            {
                assert(NULL!=b);
                push_back( other.query_(**b) );
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
        push_back( create(value) );
    }

    void io_bits:: pre(const io_bit::type value)
    {
        push_front( create(value) );
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

    std::ostream & operator<<(std::ostream &os, const io_bits &Q)
    {
#if 0
        {
            size_t counter=0;
            for(const io_bit *b = Q.head; b; b=b->next)
            {
                if(**b) os << '1'; else os << '0';
                ++counter;
                if(counter<Q.size && 0==(counter&7)) os << '-';
            }
            return os;
        }
#endif

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
            os << '[';
            while(b)
            {
                if(**b) os << '1'; else os << '0';
                b=b->next;
            }
            os << ']';
        }
        return os;
    }

    void io_bits:: release() throw()
    {
        while(size) pool.store( pop_back() );
    }

    void io_bits:: write(const char C)
    {
        std::cerr << "Append '" << C << "'" << std::endl;
        append<char>(C);
    }

    void io_bits:: flush()
    {
        static const unsigned nz[8] =
        {
            0, 7, 6, 5, 4, 3, 2, 1
        };
        for(unsigned i=nz[size&7];i>0;--i) add(_0);
        assert(0==(size%8));
    }


}
