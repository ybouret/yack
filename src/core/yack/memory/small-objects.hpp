//! \file

#ifndef YACK_SMALL_OBJECTS_INCLUDED
#define YACK_SMALL_OBJECTS_INCLUDED

#include "yack/setup.hpp"
#include "yack/memory/blocks-words.hpp"

namespace yack
{
    namespace memory
    {
        class blocks;
        class pages;

        class small_objects
        {
        public:
            static const char designation[];
            virtual ~small_objects() throw();
            explicit small_objects(const size_t the_limit_size);
            
            const size_t   limit_size;
            const uint64_t put_in_ram;
            
            void *acquire_unlocked(size_t block_size);
            void  release_unlocked(void *block_addr, size_t block_size) throw();
            void  gc_unlocked(pages &) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(small_objects);
            blocks *blk;
            void   *impl[YACK_MEMORY_BLOCKS_WORDS];
            
        };
    }
}
#endif

