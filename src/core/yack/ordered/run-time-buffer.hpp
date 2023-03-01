//! \file

#ifndef YACK_RUN_TIME_BUFFER
#define YACK_RUN_TIME_BUFFER 1

#include "yack/memory/allocator.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    


    template <typename T, typename ALLOCATOR>
    class run_time_buffer
    {
    public:
        YACK_DECL_ARGS_(T,type);

        static const bool   versatile  = true;

        inline run_time_buffer() noexcept :
        workspace(0),
        num_items(0),
        num_bytes(0)
        {
        }

        inline run_time_buffer(const size_t n) :
        workspace(0),
        num_items(n),
        num_bytes(0)
        {
            static memory::allocator &mgr = ALLOCATOR::instance();
            workspace = mgr.allocate<T>(num_items,num_bytes);
        }



        inline ~run_time_buffer() noexcept
        {
            if(workspace) {
                static memory::allocator &mgr = ALLOCATOR::location();
                mgr.withdraw(workspace,num_bytes);
                num_items = 0;
            }
        }

        inline void swap_with( run_time_buffer &other ) noexcept
        {
            cswap(workspace,other.workspace);
            cswap(num_items,other.num_items);
            cswap(num_bytes,other.num_bytes);
        }

        mutable_type *workspace;
        size_t        num_items;
        size_t        num_bytes;



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(run_time_buffer);
    };

}

#endif

