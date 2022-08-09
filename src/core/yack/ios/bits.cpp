
#include "yack/ios/bits.hpp"
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

    std::ostream & operator<<(std::ostream &os, const io_bits &Q)
    {
        for(const io_bit *b = Q.head; b; b=b->next)
        {
            if(**b) os << '1'; else os << '0';
        }
        return os;
    }

}
