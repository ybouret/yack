
//! \file

#ifndef YACK_MEMORY_SCARCE_INCLUDED
#define YACK_MEMORY_SCARCE_INCLUDED 1

#include "yack/memory/allocator.hpp"
#include "yack/memory/chunk-size.hpp"
#include "yack/singleton.hpp"
#include "yack/arith/ilog2.hpp"

namespace yack
{
    
    namespace memory
    {
        class arena;
        class theater;

        class scarce : public allocator, public singleton<scarce>
        {
        public:
            static const char               call_sign[];
            static const at_exit::longevity life_time = 0;

            static const size_t min_chunk_size = YACK_CHUNK_SIZE;
            static const size_t min_chunk_exp2 = ilog2<YACK_CHUNK_SIZE>::value;
            static const size_t min_block_size = min_chunk_size>>7;
            static const size_t min_block_exp2 = ilog2<min_block_size>::value;



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scarce);
            explicit scarce() throw();
            virtual ~scarce() throw();
        };
    
    }
    
}

#endif

