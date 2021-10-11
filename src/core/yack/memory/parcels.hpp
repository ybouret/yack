
//! \file

#ifndef YACK_MEMORY_PARCELS_INCLUDED
#define YACK_MEMORY_PARCELS_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/memory/chunk-size.hpp"
#include "yack/memory/arena-words.hpp"

namespace yack
{

    namespace memory
    {

        class parcel;
        class arena;

        class parcels
        {
        public:
            static const char designation[];
            explicit parcels();
            virtual ~parcels() throw();

            //! get/create memory >= block_size
            void *acquire_unlocked(size_t &block_size);

            //! release previously acquired block
            void  release_unlocked(void * &block_addr, size_t &block_size) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(parcels);
            parcel         *cache;
            list_of<parcel> plist;
            arena          *zpool;
            void           *impl[YACK_MEMORY_ARENA_WORDS];
            void grow_for(const size_t block_size);
            void kill(parcel *) throw();
        };


    }

}

#endif
