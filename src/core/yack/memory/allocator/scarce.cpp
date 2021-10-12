
#include "yack/memory/allocator/scarce.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/memory/chunk-size.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include <iostream>

namespace yack
{
    
    namespace memory
    {
        
        scarce:: scarce() throw()
        {
        }
        
        scarce:: ~scarce() throw()
        {
        }

        const char scarce::call_sign[] = "memory::scarce";

        static inline size_t check_min_chunk_exp2(const size_t min_chunk_exp2)
        {
            if(min_chunk_exp2>dyadic::max_page_exp2) throw libc::exception(EINVAL,"%s block size overflow",scarce::call_sign);
            return min_chunk_exp2;
        }

        size_t scarce:: chunk_exp2_for(const size_t block_exp2)
        {
            const size_t min_chunk_exp2 = check_min_chunk_exp2(block_exp2+min_bpc_exp2);
            const size_t min_chunk_size = size_t(1) << min_chunk_exp2;

            if(min_chunk_size>=YACK_CHUNK_SIZE)
            {
                assert(min_chunk_size/(1<<block_exp2)>=min_bpc_size);
                return min_chunk_exp2;
            }
            else
            {
                assert(min_chunk_size<YACK_CHUNK_SIZE);
                size_t the_chunk_exp2 = min_of(block_exp2+max_bpc_exp2,dyadic::max_page_exp2);
                size_t the_chunk_size = size_t(1) << the_chunk_exp2;
                while(the_chunk_size>YACK_CHUNK_SIZE)
                {
                    --the_chunk_exp2;
                    the_chunk_size >>= 1;
                }
                assert(the_chunk_size/(1<<block_exp2)>=min_bpc_size);
                return the_chunk_exp2;
            }
        }


    }
    
}

