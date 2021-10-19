
//! \file

#ifndef YACK_APEX_ORE_INCLUDED
#define YACK_APEX_ORE_INCLUDED 1

#include "yack/apex/m/ingot.hpp"
#include "yack/data/list.hpp"
#include "yack/memory/chunk-size.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/arith/base2.hpp"

namespace yack
{
    
    namespace apex
    {
        
        class ore
        {
        public:
            static const size_t min_block_size = YACK_CHUNK_SIZE/ingot::max_blocks;
            static const size_t min_block_exp2 = ilog2<min_block_size>::value;
            
            static const size_t max_block_exp2 = base2<size_t>::max_shift;
            static const size_t max_block_size = base2<size_t>::max_power_of_two;
            
            static size_t chunk_size_for(const size_t block_size) throw();
            
            ~ore() throw();
            ore(const size_t user_block_exp2) throw();
            
            const size_t block_exp2;
            const size_t block_size;
            const size_t chunk_size;
            const size_t new_blocks;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ore);
        };
        
    }
    
}

#endif
