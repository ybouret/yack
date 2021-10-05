
//! \file

#ifndef YACK_MEMORY_ALLOCATOR_GLOBAL_INCLUDED
#define YACK_MEMORY_ALLOCATOR_GLOBAL_INCLUDED 1

#include "yack/memory/allocator.hpp"
#include "yack/memory/allocator/global-longevity.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    namespace memory
    {
        
        class global : public allocator, public singleton<global>
        {
        public:
            static const at_exit::longevity life_time = YACK_MEMORY_GLOBAL_ALLOCATOR_LONGEVITY;
            
            virtual void *       acquire(size_t &count, const size_t block_size);
            virtual void         release(void * &addr, size_t &size)     throw();
            virtual const char * variety()                         const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(global);
            explicit global() throw();
            virtual ~global() throw();
            friend class singleton<global>;
            
        };
        
    }
}

#endif
