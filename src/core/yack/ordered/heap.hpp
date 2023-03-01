//! \file

#ifndef YACK_HEAP_INCLUDED
#define YACK_HEAP_INCLUDED 1

#include "yack/ordered/priority-queue.hpp"
#include "yack/arith/align.hpp"
#include "yack/check/static.hpp"
#include "yack/container/dynamic.hpp"

namespace yack
{

    template <typename T>
    class heap : public dynamic
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

