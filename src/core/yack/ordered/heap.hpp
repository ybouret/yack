//! \file

#ifndef YACK_HEAP_INCLUDED
#define YACK_HEAP_INCLUDED 1

#include "yack/ordered/priority-queue.hpp"
#include "yack/container.hpp"
#include "yack/container/as-copy.hpp"

namespace yack
{

    template <typename T>
    class heap 
    {
    public:
        YACK_DECL_ARGS(T,type);

        explicit heap() noexcept
        {
        }

        virtual ~heap() noexcept
        {
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(heap);

    };

}

#endif

