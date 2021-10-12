
//! \file

#ifndef YACK_MEMORY_SCARCE_INCLUDED
#define YACK_MEMORY_SCARCE_INCLUDED 1

#include "yack/memory/allocator.hpp"
#include "yack/singleton.hpp"
#include "yack/arith/ilog2.hpp"

namespace yack
{
    
    namespace memory
    {
        class scarce : public allocator, public singleton<scarce>
        {
        public:
            static const size_t min_bpc_size = 16;  //!< minimum blocks per chunk
            static const size_t max_bpc_size = 128; //!< maximum blocks per chunk, prev_power_of_two(0xff)
            static const size_t min_bpc_exp2 = ilog2<min_bpc_size>::value;
            static const size_t max_bpc_exp2 = ilog2<max_bpc_size>::value;
            static const char               call_sign[];
            static const at_exit::longevity life_time = 0;

            //! return a chunk exp2 to require from dyadic
            /**
             - chunk_size >= block_size * min_bpc_size <=> chunk_exp2 >= block_exp2+min_bpc_exp2 (<=max_page_exp2)
             - if(chunk_size<YACK_CHUNK_SIZE), grow to no more than (max_page_size,block_size*max_bpc_size)
             */
            static size_t chunk_exp2_for(const size_t block_exp2);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scarce);
            explicit scarce() throw();
            virtual ~scarce() throw();
        };
    
    }
    
}

#endif

