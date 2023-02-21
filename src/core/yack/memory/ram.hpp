//! \file

#ifndef YACK_MEMORY_RAM_INCLUDED
#define YACK_MEMORY_RAM_INCLUDED 1

#include "yack/memory/allocator.hpp"

namespace yack
{

    namespace memory
    {
        //______________________________________________________________________
        //
        //
        //! Random Access Memory, THREAD UNSAFE
        //
        //______________________________________________________________________
        class ram : public allocator
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit ram() noexcept;
            virtual ~ram() noexcept;

            //__________________________________________________________________
            //
            // allocator interface
            //__________________________________________________________________
            virtual void       *acquire(size_t &count, const size_t block_size);
            virtual void        release(void * &addr, size_t &size)     noexcept;
            virtual const char *variety()                         const noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static uint64_t allocated() noexcept; //!< monitoring
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ram);
        };

    }
    
    
}


#endif

