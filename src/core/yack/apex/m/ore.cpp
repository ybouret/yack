
#include "yack/apex/m/ore.hpp"

namespace yack
{
    
    namespace apex
    {
        ore:: ~ore() throw()
        {
        }
        
        size_t ore:: chunk_size_for(const size_t block_size) throw()
        {
            assert(block_size<=max_block_size);
            assert(block_size>=min_block_size);
            if(block_size>=YACK_CHUNK_SIZE)
            {
                
            }
        }

        
        ore:: ore(const size_t user_block_exp2) throw():
        block_exp2(user_block_exp2),
        block_size( size_t(1)  <<  block_exp2  ),
        chunk_size( chunk_size_for(block_size) ),
        new_blocks(0)
        {
        }
        
        
    }
    
}
