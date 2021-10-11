//! \file

#ifndef YACK_MEMORY_PARCEL_INCLUDED
#define YACK_MEMORY_PARCEL_INCLUDED 1

#include "yack/memory/allocator.hpp"
#include "yack/arith/ilog2.hpp"

namespace yack
{

    namespace memory
    {

        class parcel
        {
        public:
            struct stamp_t
            {
                stamp_t *prev;
                stamp_t *next;
                size_t   size;
                size_t   used;
            };

            static const size_t stamp_size    = sizeof(stamp_t);
            static const size_t stamp_exp2    = ilog2<stamp_size>::value;
            static const size_t min_data_size = 4 * stamp_size;
            static const size_t min_data_exp2 = ilog2<min_data_size>::value;

            ~parcel() throw();
            parcel(void *       block_addr,
                   const size_t block_size,
                   const size_t block_exp2 ) throw();

            stamp_t       *biggest_stamp;
            size_t         usable_stamps;
            stamp_t       *head;
            stamp_t       *tail;
            parcel        *next;
            parcel        *prev;
            const size_t   data_size;
            const size_t   data_exp2;

            void *acquire(size_t &size) throw();

            void display() const;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(parcel);

        };

    }

}

#endif

