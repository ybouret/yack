
#include "yack/apex/m/ore.hpp"
#include "yack/utest/run.hpp"
#include <cstdio>

using namespace yack;

namespace
{
    
    
}

YACK_UTEST(apex_mem)
{
    uprng ran;
    
    YACK_SIZEOF(apex::ingot);
    YACK_SIZEOF(apex::ore);
    std::cerr << "apex::ore::min_block_size=" << apex::ore::min_block_size << std::endl;
    
    void *blocks[apex::ingot::max_blocks];
    void *work[1000];
    for(size_t block_exp2=0;block_exp2<=8;++block_exp2)
    {
        const size_t block_size = size_t(1) << block_exp2;
        std::cerr << "block_size=2^" << block_exp2 << "=" << block_size << std::endl;
        
        apex::ingot s(block_size,block_exp2,work,sizeof(work));
        std::cerr << "\t#available: " << int(s.still_available) << std::endl;
        
        for(size_t iter=0;iter<1024;++iter)
        {
            memset(blocks,0,sizeof(blocks));
            for(size_t i=0;i<s.provided_number;++i)
            {
                blocks[i] = s.acquire(block_size,block_exp2);
                YACK_ASSERT(NULL!=blocks[i]);
                for(size_t j=0;j<i;++j)
                {
                    YACK_ASSERT(blocks[j]!=blocks[i]);
                }
            }
            ran.shuffle(blocks,s.provided_number);
            for(size_t i=1;i<s.provided_number;++i)
            {
                YACK_ASSERT( !s.release(blocks[i],block_size,block_exp2) );
            }
            YACK_ASSERT( s.release(blocks[0],block_size,block_exp2) );
        }
    }
    
    
}
YACK_UDONE()
