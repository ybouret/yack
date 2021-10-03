
#include "yack/data/linked.hpp"

namespace yack
{
    
    linked:: ~linked() throw()
    {
        assert(0==size);
    }
    
    linked:: linked() throw() : size(0)
    {
    }
    
    void linked:: increase() throw()
    {
        ++coerce(size);
    }
    
    void linked:: decrease() throw()
    {
        assert(size>0);
        --coerce(size);
    }
    
    void linked:: xch_size(linked &other) throw()
    {
        cswap( coerce(size), coerce(other.size) );
    }
    
}


