


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

        class book
        {
        public:
            static const size_t min_page_size = sizeof(page);
            static const size_t min_page_exp2 = ilog2<min_page_size>::value;
            static const size_t max_page_exp2 = (sizeof(size_t)<<3)-1;
            static const size_t max_page_size = size_t(1) << max_page_exp2;
            static const size_t parts         = 1+max_page_exp2-min_page_exp2;
            static const size_t bytes         = parts*sizeof(chapter);

            book() throw();
            ~book() throw();
            void *query(const size_t page_exp2);
            void  store(void *addr, const size_t page_exp2) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(book);
            chapter *chapters;
            void    *impl[ YACK_WORDS_GEQ(bytes) ];
        };

    }

}

#endif
