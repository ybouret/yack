//! \file

#ifndef YACK_APEX_VEIN_INCLUDED
#define YACK_APEX_VEIN_INCLUDED 1

#include "yack/memory/small/chunk.hpp"
#include "yack/memory/chunk-size.hpp"
#include "yack/memory/small/arena-words.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/arith/base2.hpp"
#include "yack/data/list.hpp"

namespace yack
{

    namespace memory
    {
        class arena;
    }

    namespace apex
    {

        class vein
        {
        public:
            static const size_t min_blocks = 4;
            static const size_t max_blocks = base2<uint8_t>::max_power_of_two;

            static const size_t min_block_size = YACK_CHUNK_SIZE/max_blocks; //!< never allocate less thant YACK_CHUNK_SIZE
            static const size_t lim_block_size = YACK_CHUNK_SIZE/min_blocks; //!< allocate more than YACK_CHUNK_SIZE for block_size > lim_block_slize
            static const size_t max_block_size = base2<size_t>::max_power_of_two / min_blocks;
            static const size_t min_block_exp2 = ilog2<min_block_size>::value;
            static const size_t max_block_exp2 = base2<size_t>::max_shift;


            static size_t chunk_size_for_bs(const size_t block_size) throw();

            //! block_exp2 and arena for sizeof(chunk)
            explicit vein(const size_t   usr_block_exp2,
                          memory::arena  &usr_ccache) throw();

        private:
            size_t                 available;
            list_of<memory::chunk> chunks;
            memory::arena         &ccache;

        public:
            const size_t block_exp2;
            const size_t block_size;
            const size_t chunk_size;
            const size_t new_blocks;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(vein);

            void grow();
        };

    }

}

#endif

