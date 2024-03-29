
//! \file

#ifndef YACK_MEMORY_NOTE_INCLUDED
#define YACK_MEMORY_NOTE_INCLUDED 1

#include "yack/memory/large/min-page-size.hpp"
#include "yack/memory/small/arena-words.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/arith/align.hpp"

namespace yack
{

    namespace memory
    {

        class arena;

        //______________________________________________________________________
        //
        //
        //! collection of small block_size=power_of_two arenas
        /**
         internal arenas relies on global allocator only
         */
        //
        //______________________________________________________________________
        class note
        {
        public:
            static const size_t max_page_size = YACK_MIN_PAGE_SIZE>>1;       //!< max page size
            static const size_t max_page_exp2 = ilog2<max_page_size>::value; //!< max oage exo2
            static const size_t parts         = max_page_exp2+1;             //!< number of parts

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            note();
            ~note() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void *query(const size_t page_exp2);                      //!< 0 <= page_exp2 <= max_page_exp2
            void  store(void *addr, const size_t page_exp2) noexcept;  //!< store a previously queried page
            void  display() const;                                    //!< display current info

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(note);
            arena *self;
            void  *impl[parts*YACK_MEMORY_ARENA_WORDS];
        };

    }

}

#endif

