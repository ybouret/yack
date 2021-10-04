
//! \file

#ifndef YACK_MEMORY_ALLOCATOR_INCLUDED
#define YACK_MEMORY_ALLOCATOR_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace memory
    {
        class allocator
        {
        public:
            virtual ~allocator() throw();
            
            virtual void       *acquire(size_t &count, const size_t block_size) throw() = 0;
            virtual void        release(void * &addr, size_t &size) throw() = 0;
            virtual const char *group() const throw() = 0;
            
        protected:
            explicit allocator() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(allocator);
        };
    }
    
}

#endif

