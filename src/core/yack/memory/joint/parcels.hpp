//! \file

#ifndef YACK_MEMORY_PARCELS_INCLUDED
#define YACK_MEMORY_PARCELS_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/memory/chunk-size.hpp"
#include "yack/memory/small/arena-words.hpp"

#if defined(_MSC_VER)
#include "yack/memory/joint//parcel.hpp"
#else
namespace yack { namespace memory { class parcel;  } }
#endif

namespace yack
{
    namespace memory
    {

        class arena;

        //______________________________________________________________________
        //
        //
        //! handling multiple parcels
        //
        //______________________________________________________________________
        class parcels
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char designation[]; //!< "memory::parcels";


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit parcels();          //!< setup and one default parcel
            virtual ~parcels() throw();  //!< cleanup

            
            //__________________________________________________________________
            //
            // memory
            //__________________________________________________________________
            
            //! get/create memory >= block_size
            void *acquire_unlocked(size_t &block_size);

            //! release previously acquired block
            /**
             
             */
            void  release_unlocked(void * &block_addr, size_t &block_size) throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void display() const; //!< info
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(parcels);
            parcel         *cache;
            parcel         *empty;
            list_of<parcel> plist;
            arena          *zpool;
            void           *impl[YACK_MEMORY_ARENA_WORDS];
            void  grow_for(const size_t block_size); //!< grow and set cache
            void  kill(parcel *)   throw();          //!< return to pages and zpool
            void *checked(void *)  throw();          //!< check empty!=cache or set to NULL

        };


    }

}

#endif
