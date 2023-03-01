//! \file

#ifndef YACK_RUN_TIME_BUFFER
#define YACK_RUN_TIME_BUFFER 1

#include "yack/memory/allocator.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    

    //__________________________________________________________________________
    //
    //
    //! run-time allocated buffer
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR>
    class run_time_buffer
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);             //!< aliases
        static const bool versatile  = true; //!< is versatile

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup empty
        inline run_time_buffer() noexcept :
        workspace(0),
        num_items(0),
        num_bytes(0)
        {
        }

        //! setup with capacity
        inline run_time_buffer(const size_t n) :
        workspace(0),
        num_items(n),
        num_bytes(0)
        {
            static memory::allocator &mgr = ALLOCATOR::instance();
            workspace = mgr.allocate<T>(num_items,num_bytes);
        }

        //! release resources
        inline ~run_time_buffer() noexcept
        {
            if(workspace) {
                static memory::allocator &mgr = ALLOCATOR::location();
                mgr.withdraw(workspace,num_bytes);
                num_items = 0;
            }
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! no-throw swap
        inline void swap_with( run_time_buffer &other ) noexcept
        {
            cswap(workspace,other.workspace);
            cswap(num_items,other.num_items);
            cswap(num_bytes,other.num_bytes);
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        mutable_type *workspace; //!< address of first item
        size_t        num_items; //!< number of items
        size_t        num_bytes; //!< number of bytes



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(run_time_buffer);
    };

}

#endif

