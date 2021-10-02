
#include "yack/memory/arena.hpp"
#include "yack/memory/chunk.hpp"
#include "yack/arith/base2.hpp"
#include <iostream>

namespace yack
{
    
    namespace memory
    {
        arena:: ~arena() throw()
        {
            
        }
        
        arena:: arena(const size_t block_size) throw() :
        available_chunks(0),
        chunks(),
        chunk_block_size(block_size),
        blocks_per_chunk(0),
        memory_per_chunk( chunk::optimized_bytes_for(block_size, coerce(blocks_per_chunk) ) ),
        memory_signature( base2<size_t>::log2_of(memory_per_chunk) )
        {
            std::cerr << "<arena>" << std::endl;
            std::cerr << "  chunk_block_size: " << chunk_block_size << std::endl;
            std::cerr << "  blocks_per_chunk: " << blocks_per_chunk << std::endl;
            std::cerr << "  memory_per_chunk: " << memory_per_chunk << std::endl;
            std::cerr << "  memory_signature: " << memory_signature << std::endl;
            std::cerr << "<arena/>" << std::endl;
            
            memset(chunks,0,sizeof(chunks));
            
        }
        
    }
    
}



