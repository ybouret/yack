
//! \file

#ifndef YACK_MEMORY_ZCACHE_INCLUDED
#define YACK_MEMORY_ZCACHE_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/data/core-pool.hpp"
#include "yack/memory/small/arena-words.hpp"

namespace yack
{
    
    namespace memory
    {
        class arena;
        
        class zcache
        {
        public:
            struct stub
            {
                stub *next;
                stub *prev;
            };
            
            static size_t block_size_for(const size_t bs) throw();
            
            explicit zcache(const size_t bs);
            virtual ~zcache() throw();
            
            void  load(size_t);   //!< well -> repo
            void  free() throw(); //!< repo -> well
            void *acquire_unlocked();
            void  release_unlocked(void *) throw();
        
            const size_t       block_size;
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zcache);
            core_pool_of<stub> repo;
            arena             *well;
            void              *impl[YACK_MEMORY_ARENA_WORDS];
        };
    }
    
}


#endif

