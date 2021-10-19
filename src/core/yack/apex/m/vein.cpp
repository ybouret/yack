#include "yack/apex/m/vein.hpp"
#include "yack/type/utils.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/object.hpp"

namespace yack
{

    namespace apex
    {
        size_t vein:: chunk_size_for_bs(const size_t block_size) throw()
        {
            assert(block_size>=min_block_size);
            assert(block_size<=max_block_size);
            assert(is_a_power_of_two(block_size));

            if(block_size<=lim_block_size)
            {
                return YACK_CHUNK_SIZE;
            }
            else
            {
                return min_block_size * YACK_CHUNK_SIZE;
            }


        }


        vein:: vein(const size_t usr_block_exp2) throw() :
        block_exp2(usr_block_exp2),
        block_size( size_t(1) << block_exp2 ),
        chunk_size( chunk_size_for_bs(block_size) ),
        new_blocks(chunk_size/block_size),
        available(0),
        chunks()
        {
        }


        void vein:: grow()
        {
            static memory::global &mgr = memory::global::instance();
            assert(available<=0);
            size_t required = 1;
            void  *data = mgr.acquire(required,chunk_size); assert(chunk_size==required);
            //memory::chunk *node = object::zacquire<memory::chunk>();

        }
    }

}
