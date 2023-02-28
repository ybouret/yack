
//! \file

#ifndef YACK_HEAP_INCLUDED
#define YACK_HEAP_INCLUDED 1

#include "yack/ordered/priority-queue.hpp"
#include "yack/arith/align.hpp"
#include "yack/check/static.hpp"
#include "yack/container/dynamic.hpp"

namespace yack
{

    template <typename T, const size_t N>
    class heap : public dynamic
    {
    public:
        YACK_DECL_ARGS(T,type);
        static const size_t type_size = sizeof(type);
        static const size_t wksp_size = type_size * N;

        explicit heap() noexcept
        {
            YACK_STATIC_CHECK(sizeof(wksp)>=wksp_size,wksp_too_small);
        }

        virtual ~heap() noexcept
        {
        }

        inline virtual size_t granted() const noexcept { return sizeof(wksp); }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(heap);
        void *wksp[ YACK_WORDS_GEQ(wksp_size) ];
        static inline int compare(const_type &lhs, const_type &rhs) noexcept {
            return lhs < rhs ? -1 : 1;
        }
    };

}

#endif

