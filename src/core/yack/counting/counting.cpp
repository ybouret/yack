#include "yack/counting/counting.hpp"
#include "yack/exception.hpp"

namespace yack
{
    
    counting:: counting(const cardinality_t num) throw() :
    index(1),
    total(num)
    {
        assert(num>0);
    }
    
    counting:: ~counting() throw()
    {
    }
}

