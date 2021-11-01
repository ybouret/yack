//! \file

#ifndef YACK_MEMORY_ZCACHE_INCLUDED
#define YACK_MEMORY_ZCACHE_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/data/pool/core.hpp"
#include "yack/memory/small/arena-words.hpp"

namespace yack
{
    
    namespace memory
    {
        class arena;

        //______________________________________________________________________
        //
        //
        //! cache of zombie data
        /**
         hold a local cache of objects of same sizes
         */
        //
        //______________________________________________________________________
        class zcache
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! stub to handle memory
            struct stub
            {
                stub *next; //!< for list/pool
                stub *prev; //!< for list
            };

            //! compute the block_size>=bs
            static size_t block_size_for(const size_t bs) throw();

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit zcache(const size_t bs); //!< setup
            virtual ~zcache() throw();        //!< cleanup

            //__________________________________________________________________
            //
            // repository helpers
            //__________________________________________________________________
            void  load(size_t);   //!< well -> repo
            void  free() throw(); //!< repo -> well

            //__________________________________________________________________
            //
            // blocks I/O
            //__________________________________________________________________
            void *acquire_unlocked();               //!< from repo or well
            void  release_unlocked(void *) throw(); //!< to repo

            static void xdisabled(const char *clid); //!< system error
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t       block_size;         //!< the block size for all
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zcache);
            core_pool_of<stub> repo;
            arena             *well;
            void              *impl[YACK_MEMORY_ARENA_WORDS];
        };
    }
    
}


#endif

