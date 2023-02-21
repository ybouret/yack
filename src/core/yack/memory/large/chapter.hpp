


//! \file

#ifndef YACK_MEMORY_CHAPTER_INCLUDED
#define YACK_MEMORY_CHAPTER_INCLUDED 1

#include "yack/memory/large/page.hpp"
#include "yack/data/list.hpp"

namespace yack
{

    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! chapters of pages
        //
        //______________________________________________________________________
        class chapter : public list_of<page>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit chapter(const size_t sz) noexcept; //!< initialize with page_size
            virtual ~chapter() noexcept;                //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void  *query();               //!< recall/create a page
            void   store(void *) noexcept; //!< store a former page
            void   display() const;       //!< display status


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t page_size;  //!< page size, a power of two

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(chapter);
        };


    }

}

#endif
