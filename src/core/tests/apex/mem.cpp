
#include "yack/apex/m/hoard.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include <cstdio>

using namespace yack;

namespace
{

    struct block
    {
        void   *addr;
        size_t  exp2;
    };
    
}

YACK_UTEST(apex_mem)
{
    randomized::rand_  ran;
    YACK_SIZEOF(apex::hoard::piece);
    YACK_SIZEOF(apex::hoard);
    YACK_SIZEOF(apex::hoard::repository);

    YACK_OUTPUT(apex::hoard::min_block_size);
    YACK_OUTPUT(apex::hoard::min_block_exp2);
    YACK_OUTPUT(apex::hoard::max_block_size);
    YACK_OUTPUT(apex::hoard::max_block_exp2);
    YACK_OUTPUT(apex::hoard::repositories);
    YACK_OUTPUT(apex::hoard::repo_raw_bytes);
    YACK_OUTPUT(apex::hoard::repo_bytes);
    YACK_OUTPUT(apex::hoard::repo_words);


    block blocks[1000];
    const size_t nblock = sizeof(blocks)/sizeof(blocks[0]);
    out_of_reach::zset(blocks,sizeof(blocks));

    {
        apex::hoard H;

        for(size_t iter=0;iter<128;++iter)
        {
            for(size_t i=0;i<nblock;++i)
            {
                const size_t org = ran.leq(10);
                block       &blk = blocks[i];
                blk.addr  = H.acquire_unlocked( (blk.exp2=org) );
                YACK_ASSERT(blk.exp2>=apex::hoard::min_block_exp2);
            }
            randomized::shuffle::data(blocks,nblock,ran);
            for(size_t i=0;i<nblock;++i)
            {
                block       &blk = blocks[i];
                H.release_unlocked(blk.addr,blk.exp2);
            }
        }
        H.display();
    }

    memory::dyadic::instance().display();



}
YACK_UDONE()
