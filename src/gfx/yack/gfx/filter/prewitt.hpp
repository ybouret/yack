//! \file
#ifndef YACK_GFX_FILTER_PREWITT_INCLUDED
#define YACK_GFX_FILTER_PREWITT_INCLUDED 1

#include "yack/gfx/filter/decl.hpp"

namespace yack
{
    namespace graphic
    {
        template <unit_t N>
        struct Prewitt
        {
            static const unit_t size = N;
            static const char   uuid[];
            static const int    data[N][N];
        };

#if !defined(_MSC_VER)
        YACK_GFX_FILTER_DECL(Prewitt,3);
        YACK_GFX_FILTER_DECL(Prewitt,5);
        YACK_GFX_FILTER_DECL(Prewitt,7);
#endif

    }
}

#endif

