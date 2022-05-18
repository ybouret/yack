//! \file

#ifndef YACK_GFX_FILTER_SOBEL_INCLUDED
#define YACK_GFX_FILTER_SOBEL_INCLUDED 1

#include "yack/gfx/filter/decl.hpp"

namespace yack
{
    namespace graphic
    {
        //! Sobel's filters
        template <unit_t N>
        struct Sobel
        {
            static const unit_t size = N;   //!< NxN
            static const char   uuid[];     //!< "SobelNxN"
            static const int    data[N][N]; //!< data
        };

#if !defined(_MSC_VER)
        YACK_GFX_FILTER_DECL(Sobel,3); //!< declare
        YACK_GFX_FILTER_DECL(Sobel,5); //!< declare
        YACK_GFX_FILTER_DECL(Sobel,7); //!< declare
#endif

    }
}

#endif

