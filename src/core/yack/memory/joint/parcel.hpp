//! \file

#ifndef YACK_MEMORY_PARCEL_INCLUDED
#define YACK_MEMORY_PARCEL_INCLUDED 1

#include "yack/memory/allocator.hpp"
#include "yack/arith/ilog2.hpp"

namespace yack
{

    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! low-level stamped memory within a parcel
        //
        //______________________________________________________________________
        class parcel
        {
        public:
            //__________________________________________________________________
            //
            //! parcel internal marker and unit_block
            //__________________________________________________________________
            struct stamp_t
            {
                stamp_t *prev; //!< internal list
                stamp_t *next; //!< internal list
                size_t   size; //!< available blocks
                parcel  *user; //!< NULL:free, not null:used
            };

            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const size_t stamp_size    = sizeof(stamp_t);             //!< alias
            static const size_t stamp_exp2    = ilog2<stamp_size>::value;    //!< alias
            static const size_t stamp_lost    = 2*stamp_size;                //!< room for head+tail
            static const size_t min_stamps    = 4;                           //!< alias
            static const size_t min_data_size = min_stamps * stamp_size;     //!< alias
            static const size_t min_data_exp2 = ilog2<min_data_size>::value; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup: display memory leaks
            ~parcel() throw();

            //! setup from a power of two memory block
            parcel(void *       block_addr,
                   const size_t block_size,
                   const size_t block_exp2 ) throw();


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void                 *try_acquire(size_t &size)                     throw(); //!< try acquire a piece of memory
            static parcel        *get_release(void * &entry, size_t &count)     throw(); //!< release old memory, return owner
            bool                  is_empty()                              const throw(); //!< check vacancy condition
            size_t                capacity()                              const throw(); //!< max available space
            void                  display()                                       const; //!< info
            static const parcel  *whose(const void *entry)                      throw(); //!< to check ownership
            static int            compare(const parcel *lhs, const parcel *rhs) throw(); //!< comparison by size then address
            static size_t         size_of(const void *addr)                     throw(); //!< retrieve allocated size

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            stamp_t       *head; //!< first stamp at block_addr
            stamp_t       *tail; //!< sentinel block at end of block_addr
            parcel        *next; //!< for linked list
            parcel        *prev; //!< for linked list
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(parcel);

        };

    }

}

#endif

