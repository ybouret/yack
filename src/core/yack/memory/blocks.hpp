
//! \file

#ifndef YACK_MEMORY_BLOCKS_INCLUDED
#define YACK_MEMORY_BLOCKS_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/memory/arena-words.hpp"

namespace yack
{

    namespace memory
    {
        //______________________________________________________________________
        //
        //
        // forward declarations
        //
        //______________________________________________________________________
        class arena;
        class pages;

        //______________________________________________________________________
        //
        //
        //! on demand acquire/release blocks of memory
        /**
         - for each block_size, a dedicated arena is allocated
         - the arenas are stored in a hash_table
         - internal dependencies
         */
        //
        //______________________________________________________________________
        class blocks
        {
        public:
            static const size_t    arena_words = YACK_MEMORY_ARENA_WORDS;   //!< internal memory
            static const char      designation[];      //!< memory blocks
            typedef list_of<arena> niche_type;         //!< niche for table

            blocks();           //!< setup with capacity but not arena
            ~blocks() throw();  //!< cleanup

            void *acquire(const size_t block_size);                           //!< acquire block
            void  release(void *block_addr, const size_t block_size) throw(); //!< release block
            void  gc(pages &) throw();                                        //!< garbage collection for each arena

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(blocks);
            arena           *acquiring_arena;    //!< last acquiring arena
            arena           *releasing_arena;    //!< last releasing arena
            niche_type      *acquiring_niche;    //!< last acquiring niche
            niche_type      *releasing_niche;    //!< last releasing niche

            size_t           count;              //!< number or recorded arenas
            const size_t     tsize;              //!< table size, power of two
            const size_t     tmask;              //!< table size - 1, for table access
            const size_t     bytes;              //!< table bytes
            niche_type      *table;              //!< table address
            void            *impl_[arena_words]; //!< arena creator

            void   release_table() throw();
            void   grow(const size_t block_size, niche_type *);
            arena *find(niche_type *, const size_t block_size) throw();
            bool   check(const niche_type *slot) const throw();
            
        };

    }

}

#endif
