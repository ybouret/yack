
#include "yack/memory/ram.hpp"
#include "yack/memory/arena.hpp"
#include "yack/system/exception.hpp"
#include "yack/utest/run.hpp"
#include <cstring>
#include <cerrno>

using namespace yack;

 

YACK_UTEST(memory_arena)
{
    
    for(size_t block_size=1;block_size<=16;++block_size)
    {
        memory::arena a(block_size);
        
    }
   
}
YACK_UDONE()

