//! \file
#ifndef YACK_GFX_FILTER_PREWITT_INCLUDED
#define YACK_GFX_FILTER_PREWITT_INCLUDED 1

#include "yack/gfx/filter/decl.hpp"

namespace yack
{
    namespace graphic
    {
        //! Prewitt's filters
        template <unit_t N>
        struct Prewitt
        {
            static const unit_t size = N;   //!< NxN
            static const char   uuid[];     //!< "PrewittNxN"
            static const int    data[N][N]; //!< data
        };

#if !defined(_MSC_VER)
        YACK_GFX_FILTER_DECL(Prewitt,3); //!< declare
        YACK_GFX_FILTER_DECL(Prewitt,5); //!< declare
        YACK_GFX_FILTER_DECL(Prewitt,7); //!< delcare
#endif

    }
}

#endif

