

#include "yack/memory/allocator/scarce.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/arena.hpp"
#include "yack/memory/chunk.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"


using namespace yack;

YACK_UTEST(memory_scarce)
{
    memory::allocator &mgr = memory::global::instance();
    
    for(size_t block_exp2=0,block_size=1;block_exp2<=16;++block_exp2,block_size<<=1)
    {
        memory::arena a(block_size,mgr,true);
        memory::arena b(block_size,mgr,false);
        
        a.display();
        b.display();
        std::cerr << std::endl;
    }
    
    std::cerr << "chunk: " << memory::chunk::header << std::endl;
    
}
YACK_UDONE()

