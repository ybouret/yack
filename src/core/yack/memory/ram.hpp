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
            explicit ram() throw();
            virtual ~ram() throw();

            //__________________________________________________________________
            //
            // allocator interface
            //__________________________________________________________________
            virtual void       *acquire(size_t &count, const size_t block_size);
            virtual void        release(void * &addr, size_t &size)     throw();
            virtual const char *variety()                         const throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static uint64_t allocated() throw(); //!< monitoring
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ram);
        };

    }
    
    
}


#endif

