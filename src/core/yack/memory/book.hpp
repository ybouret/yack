


//! \file

#ifndef YACK_MEMORY_BOOK_INCLUDED
#define YACK_MEMORY_BOOK_INCLUDED 1

#include "yack/memory/chapter.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/arith/align.hpp"

namespace yack
{

    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! book of chapters
        //
        //______________________________________________________________________
        class book
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const size_t min_page_size = sizeof(page);                  //!< mininal page size
            static const size_t min_page_exp2 = ilog2<min_page_size>::value;   //!< minimal page exp2
            static const size_t max_page_exp2 = (sizeof(size_t)<<3)-1;         //!< maximal page exp2
            static const size_t max_page_size = size_t(1) << max_page_exp2;    //!< maximal page size
            static const size_t parts         = 1+max_page_exp2-min_page_exp2; //!< active parts
            static const size_t bytes         = parts*sizeof(chapter);         //!< internal bytes

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            book() throw();  //!< setup chapters
            ~book() throw(); //!< clean all

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void *query(const size_t page_exp2);                      //!< min_page_exp2 <= page_exp2 <= max_page_exp2
            void  store(void *addr, const size_t page_exp2) throw();  //!< store a previously queried page

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(book);
            chapter *chapters;
            void    *impl[ YACK_WORDS_GEQ(bytes) ];
        };

    }

}

#endif
