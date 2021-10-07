


//! \file

#ifndef YACK_MEMORY_BOOK_INCLUDED
#define YACK_MEMORY_BOOK_INCLUDED 1

#include "yack/memory/chapter.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/arith/align.hpp"
#include "yack/memory/arena-words.hpp"

namespace yack
{

    namespace memory
    {

        class arena;

        //______________________________________________________________________
        //
        //
        //! book of power_of_two parts
        /**
         - large parts are chapter
         - small parts are arena
         - underlying global allocator
         */
        //
        //______________________________________________________________________
        class book
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const size_t min_page_size = sizeof(page);                          //!< mininal page size
            static const size_t min_page_exp2 = ilog2<min_page_size>::value;           //!< minimal page exp2
            static const size_t max_page_exp2 = (sizeof(size_t)<<3)-1;                 //!< maximal page exp2
            static const size_t max_page_size = size_t(1) << max_page_exp2;            //!< maximal page size
            static const size_t large_parts   = 1+max_page_exp2-min_page_exp2;         //!< active large parts
            static const size_t large_bytes   = large_parts*sizeof(chapter);           //!< active large bytes
            static const size_t large_words   = YACK_WORDS_GEQ(large_bytes);           //!< words for large parts
            static const size_t small_parts   = min_page_exp2;                         //!< active small parts
            static const size_t small_words   = YACK_MEMORY_ARENA_WORDS * small_parts; //!< words for small parts

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            book();          //!< setup chapters and arena
            ~book() throw(); //!< clean all

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void *query(const size_t page_exp2);                      //!< min_page_exp2 <= page_exp2 <= max_page_exp2
            void  store(void *addr, const size_t page_exp2) throw();  //!< store a previously queried page
            void  display() throw();                                  //!< display current info
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(book);
            chapter *chapters; //!< [min_page_exp2:max_page_exp2]
            arena   *arenas;   //!< [0:min_page_exp2-1]
            void    *large_impl[large_words];
            void    *small_impl[small_words];
        };

    }

}

#endif
