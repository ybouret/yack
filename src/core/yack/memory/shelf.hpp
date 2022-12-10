//! \file

#ifndef YACK_MEMORY_SHELF_INCLUDED
#define YACK_MEMORY_SHELF_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace memory
    {
        class allocator;
        class embed;
        
        //! used to postpone resource acquisition
        class shelf
        {
        public:
            shelf()  throw();
            shelf(const void  *addr,
                  const size_t size,
                  allocator   &user) throw();
            ~shelf() throw();
            
            void trade(shelf &other) throw();
            void empty() throw();
            void store(const void  *addr,
                       const size_t size,
                       allocator   &user) throw();
            void build(embed        emb[],
                       const size_t num,
                       allocator   &user);
            
            
            const size_t bytes;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(shelf);
            void *       entry;
            allocator   *owner;
        };
        
    }
}

#endif

