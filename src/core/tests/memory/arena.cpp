
#include "yack/memory/ram.hpp"
#include "yack/memory/arena.hpp"
#include "yack/system/exception.hpp"
#include "yack/utest/run.hpp"
#include <cstring>
#include <cerrno>

using namespace yack;



YACK_UTEST(memory_arena)
{
    
    uprng        ran;
    void *       blocks[1024];
    const size_t nblock = sizeof(blocks)/sizeof(blocks[0]);
    
    for(size_t block_size=1;block_size<=16;++block_size)
    {
        memory::arena a(block_size);
        for(size_t i=0;i<nblock;++i)
        {
            blocks[i] = a.acquire();
        }
        ran.shuffle(blocks,nblock);
        for(size_t i=0;i<nblock/2;++i)
        {
            a.release(blocks[i]);
        }
        for(size_t i=0;i<nblock/2;++i)
        {
            blocks[i] = a.acquire();
        }
        ran.shuffle(blocks,nblock);
        for(size_t i=0;i<nblock;++i)
        {
            a.release(blocks[i]);
        }
    }
   
    YACK_SIZEOF(memory::arena);
}
YACK_UDONE()

