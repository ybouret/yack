

#include "yack/memory/allocator/scarce.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/arena.hpp"
#include "yack/memory/chunk.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"


using namespace yack;

YACK_UTEST(memory_scarce)
{
    //memory::allocator &mgr = memory::global::instance();
    
    for(size_t block_exp2=0,block_size=1;block_exp2<=20;++block_exp2,block_size<<=1)
    {
        const size_t chunk_exp2 = memory::scarce::chunk_exp2_for(block_exp2);
        const size_t chunk_size = size_t(1) << chunk_exp2;
        std::cerr << "block_size=" << block_size  <<"=2^" << block_exp2;
        std::cerr << " => chunk_size=" << chunk_size << "=2^" << chunk_exp2;
        std::cerr<< std::endl;
    }
    

}
YACK_UDONE()

