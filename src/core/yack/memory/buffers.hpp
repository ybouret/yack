
//! \file

#ifndef YACK_MEMORY_BUFFERS_INCLUDED
#define YACK_MEMORY_BUFFERS_INCLUDED 1

#include "yack/memory/buffer/rw.hpp"
#include "yack/container/writable.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! specialized buffer
        //
        //______________________________________________________________________
        template <typename T,typename ALLOCATOR>
        class buffer_of : public rw_buffer, public writable<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type); //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! deallocate
            inline virtual ~buffer_of() noexcept
            {
                static ALLOCATOR &mgr = ALLOCATOR::location();
                mgr.template withdraw<mutable_type>(addr,bytes);
                items = 0;
                item  = 0;
            }

            //! allocated
            inline explicit buffer_of(const size_t n) :
            items(n),
            bytes(0),
            addr( ALLOCATOR::instance().template allocate<mutable_type>(items,bytes) ),
            item( addr-1 )
            {
            }

            //__________________________________________________________________
            //
            // buffer interface
            //__________________________________________________________________
            inline virtual size_t      measure() const noexcept { return bytes; } //!< allocate bytes
            inline virtual const void *ro_addr() const noexcept { return addr;  } //!< address

            //__________________________________________________________________
            //
            // writable interface
            //__________________________________________________________________
            inline virtual size_t       size() const noexcept { return items; }
            inline virtual const_type & operator[](const size_t indx) const noexcept { assert(indx>0); assert(indx<=size()); return item[indx]; }
            inline virtual type       & operator[](const size_t indx)       noexcept { assert(indx>0); assert(indx<=size()); return item[indx]; }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            inline type       * operator*() noexcept       { return addr; } //!< access
            inline const_type * operator*() const noexcept { return addr; } //!< access const

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(buffer_of);
            size_t        items;
            size_t        bytes;
            mutable_type *addr;
            mutable_type *item;
        };


    }

}

#endif
