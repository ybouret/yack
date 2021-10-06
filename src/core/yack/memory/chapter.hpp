


//! \file

#ifndef YACK_MEMORY_CHAPTER_INCLUDED
#define YACK_MEMORY_CHAPTER_INCLUDED 1

#include "yack/memory/page.hpp"
#include "yack/data/list.hpp"

namespace yack
{

    namespace memory
    {

        class chapter : public list_of<page>
        {
        public:
            explicit chapter(const size_t sz) throw();
            virtual ~chapter() throw();

            //const size_t page_exp2;
            const size_t page_size;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(chapter);
        };


    }

}

#endif
