
#include "yack/memory/allocator/scarce.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/memory/chunk-size.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include <iostream>

namespace yack
{
    
    namespace memory
    {
        
        scarce:: scarce() throw()
        {
        }
        
        scarce:: ~scarce() throw()
        {
        }

        const char scarce::call_sign[] = "memory::scarce";

        
        

    }
    
}

