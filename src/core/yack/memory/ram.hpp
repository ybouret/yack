
//! \file

#ifndef YACK_MEMORY_RAM_INCLUDED
#define YACK_MEMORY_RAM_INCLUDED 1

#include "yack/memory/allocator.hpp"

namespace yack
{

    namespace memory
    {

        class ram : public allocator
        {
        public:
            explicit ram() throw();
            virtual ~ram() throw();

            virtual void       *acquire(size_t &count, const size_t block_size);
            virtual void        release(void * &addr, size_t &size)     throw();
            virtual const char *variety()                         const throw();

            uint64_t get() const throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ram);
        };

    }
    
    
}


#endif

