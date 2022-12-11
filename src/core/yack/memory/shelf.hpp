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

        //______________________________________________________________________
        //
        //
        //! used to postpone resource acquisition on a linear memory space
        //
        //______________________________________________________________________
        class shelf
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            shelf()  throw(); //!< default empty constructor
            ~shelf() throw(); //!< cleanup

            //! assign allocated memory and allocator to release it
            shelf(const void  *addr,
                  const size_t size,
                  allocator   &user) throw();
            


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void trade(shelf &other) throw(); //!< no-throw trade contents
            void empty()             throw(); //!< release linear memory
            void store(const void  *addr,
                       const size_t size,
                       allocator   &user) throw(); //!< assign by copy/tread
            void build(embed        emb[],
                       const size_t num,
                       allocator   &user); //!< store block addr/size of embedded scheme
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t bytes; //!< current linear bytes
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(shelf);
            void *       entry;
            allocator   *owner;
        };
        
    }
}

#endif

