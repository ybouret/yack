//! \file

#ifndef YACK_MEMORY_DYADIC_ALLOCATOR_INCLUDED
#define YACK_MEMORY_DYADIC_ALLOCATOR_INCLUDED 1

#include "yack/memory/allocator.hpp"
#include "yack/memory/allocator/dyadic-longevity.hpp"
#include "yack/singleton.hpp"
#include "yack/data/pool/core.hpp"

namespace yack
{
    
    namespace memory
    {

        class  note;
        class  book;
        class  chunk;
 
        //______________________________________________________________________
        //
        //
        //! thread safe global memory allocator
        //
        //______________________________________________________________________
        class dyadic : public allocator, public singleton<dyadic>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! the maximum available longevity
            static const at_exit::longevity life_time = YACK_MEMORY_DYADIC_ALLOCATOR_LONGEVITY;
            static const char               call_sign[];   //!< "memory::pages"
            static const size_t             max_page_size; //!< book::max_page_size
            static const size_t             min_page_size; //!< book::min_page_size
            static const size_t             max_page_exp2; //!< book::max_page_exp2
            static const size_t             min_page_exp2; //!< book::min_page_exp2

            //__________________________________________________________________
            //
            // allocator interface
            //__________________________________________________________________
            virtual void *       acquire(size_t &count, const size_t block_size);
            virtual void         release(void * &addr, size_t &size)     noexcept;
            virtual const char * variety()                         const noexcept;

            //__________________________________________________________________
            //
            // specific interface
            //__________________________________________________________________
            void *query(const size_t page_exp2);                     //!< from internal note+book
            void  store(void *addr, const size_t page_exp2) noexcept; //!< from internal note+book
            void  display() const;                                   //!< display current info
            
            //! store chunks with same page_exp2
            void  retrieve(core_pool_of<chunk> &reservoir,
                           const size_t         page_exp2) noexcept;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(dyadic);
            explicit dyadic();
            virtual ~dyadic() noexcept;
            note    *note_; //!< [1..book::min_page_exp2-1]
            book    *book_; //!< [min_page_exp2:max_page_exp2]
            friend class singleton<dyadic>;
        };

    }
}

#endif
