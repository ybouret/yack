

//! \file

#ifndef YACK_MEMORY_ALLOCATOR_POOLED_INCLUDED
#define YACK_MEMORY_ALLOCATOR_POOLED_INCLUDED 1

#include "yack/memory/allocator.hpp"
#include "yack/memory/allocator/pooled-longevity.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    namespace memory
    {
        
        //______________________________________________________________________
        //
        //
        //! thread safe global memory allocator
        //
        //______________________________________________________________________
        class pooled : public allocator, public singleton<pooled>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            //! the maximum available longevity
            static const at_exit::longevity life_time = YACK_MEMORY_POOLED_ALLOCATOR_LONGEVITY;
            static const char               call_sign[]; //!< memory::pooled
            
            //__________________________________________________________________
            //
            // allocator interface
            //__________________________________________________________________
            virtual void *       acquire(size_t &count, const size_t block_size);
            virtual void         release(void * &addr, size_t &size)     throw();
            virtual const char * variety()                         const throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pooled);
            explicit pooled();
            virtual ~pooled() throw();
            friend class singleton<pooled>;
        };
        
    }
}

#endif
