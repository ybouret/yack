

#include "yack/memory/allocator/scarce.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/arena.hpp"
#include "yack/memory/chunk.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"


using namespace yack;

#define SHOW(var) std::cerr << std::setw(32) << "sizeof(" #var ") =" << var << std::endl

YACK_UTEST(memory_scarce)
{
    SHOW(memory::scarce::min_chunk_size);
    SHOW(memory::scarce::min_chunk_exp2);
    SHOW(memory::scarce::min_block_size);
    SHOW(memory::scarce::min_block_exp2);

    for(size_t block_exp2=0,block_size=1;block_exp2<=20;++block_exp2,block_size<<=1)
    {
        
    }
    

}
YACK_UDONE()

