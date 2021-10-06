

//! \file

#ifndef YACK_MEMORY_ALLOCATOR_PAGES_INCLUDED
#define YACK_MEMORY_ALLOCATOR_PAGES_INCLUDED 1

#include "yack/memory/allocator.hpp"
#include "yack/memory/allocator/pages-longevity.hpp"
#include "yack/memory/book.hpp"
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
        class pages : public allocator, public singleton<pages>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! the maximum available longevity
            static const at_exit::longevity life_time = YACK_MEMORY_PAGES_ALLOCATOR_LONGEVITY;
            static const char               call_sign[]; //!< "memory::pages"
            static const size_t             max_page_size; //!< book::max_page_size
            static const size_t             min_page_size; //!< book::min_page_size


            //__________________________________________________________________
            //
            // allocator interface
            //__________________________________________________________________
            virtual void *       acquire(size_t &count, const size_t block_size);
            virtual void         release(void * &addr, size_t &size)     throw();
            virtual const char * variety()                         const throw();

            //__________________________________________________________________
            //
            // specific interface
            //__________________________________________________________________
            void *query(const size_t page_exp2);                     //!< from internal book
            void  store(void *addr, const size_t page_exp2) throw(); //!< from internal book
            void  display() const;                                   //!< display current info

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pages);
            explicit pages() throw();
            virtual ~pages() throw();
            book    *tome;
            friend class singleton<pages>;
        };

    }
}

#endif
