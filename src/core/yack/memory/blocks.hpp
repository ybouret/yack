
//! \file

#ifndef YACK_MEMORY_BLOCKS_INCLUDED
#define YACK_MEMORY_BLOCKS_INCLUDED 1

#include "yack/data/list.hpp"

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

        //______________________________________________________________________
        //
        //
        //! handling blocks
        //
        //______________________________________________________________________
        class blocks
        {
        public:
            static const size_t    arena_words = 16;   //!<
            static const char      designation[];      //!< memory blocks
            typedef list_of<arena> slot_type;          //!< slot for table

            explicit blocks();          //!< setup with capacity but not arena
            virtual ~blocks() throw();  //!< cleanup

            void *acquire(const size_t block_size);                           //!< acquire block
            void  release(void *block_addr, const size_t block_size) throw(); //!< release block

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(blocks);
            arena           *acquiring_arena; //!< last acquiring
            arena           *releasing_arena; //!< last releasing
            slot_type       *acquiring_slot;
            slot_type       *releasing_slot;
            size_t           count; //!< number or recorded arenas
            const size_t     tsize; //!< table size, power of two
            const size_t     tmask; //!< table size - 1, for table access
            const size_t     bytes; //!< table bytes
            slot_type       *table; //!< table address
            void            *impl_[arena_words]; //!< arena creator

            void   release_table() throw();
            void   grow(const size_t block_size, slot_type *slot);
            arena *find(slot_type *slot, const size_t block_size) throw();

            bool   check(const slot_type *slot) const throw();
        };

    }

}

#endif
