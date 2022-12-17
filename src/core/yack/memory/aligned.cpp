
#include "yack/memory/aligned.hpp"
#include "yack/arith/base2.hpp"
#include "yack/arith/align.hpp"

namespace yack
{
    namespace memory
    {
        const size_t memory::aligned::largest = base2<size_t>::max_power_of_two;
        
        size_t memory::aligned::package(const size_t n) throw()
        {
            assert(n<=largest);
            return YACK_MEMALIGN(n);
        }
        
    }
    
}

