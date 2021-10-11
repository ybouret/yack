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
        class dyadic;

        //______________________________________________________________________
        //
        //
        //! internal handling of small blocks
        /**
         block_size <= limit_size -> arenas
         block_size  > limit_size -> put in ram
         the internal global memory is thread safe
         */
        //
        //______________________________________________________________________
        class small_objects
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~small_objects() throw();                     //!< cleanup
            explicit small_objects(const size_t the_limit_size);  //!< setup

            //__________________________________________________________________
            //
            // thread unsafe metjods
            //__________________________________________________________________
            void *acquire_unlocked(size_t block_size);                           //!< acquire a new block
            void  release_unlocked(void *block_addr, size_t block_size) throw(); //!< release an old block
            void  gc_unlocked(dyadic &) throw();                                 //!< garbage collection

            const size_t      limit_size;    //!< to choose allocation methods
            const uint64_t    put_in_ram;    //!< bookkeeping or large alloc
            static const char designation[]; //!< "memory::small_objects"

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(small_objects);
            blocks *blk;
            void   *impl[YACK_MEMORY_BLOCKS_WORDS];
            
        };
    }
}
#endif

