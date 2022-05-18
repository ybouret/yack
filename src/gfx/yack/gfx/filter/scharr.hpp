//! \file

#ifndef YACK_GFX_FILTER_SCHARR_INCLUDED
#define YACK_GFX_FILTER_SCHARR_INCLUDED 1

#include "yack/gfx/filter/decl.hpp"

namespace yack
{
    namespace graphic
    {
        //! Scharr's filters
        template <unit_t N>
        struct Scharr
        {
            static const unit_t size = N;    //!< NxN
            static const char   uuid[];      //!< "ScharrNxN"
            static const int    data[N][N];  //!< data
        };

#if !defined(_MSC_VER)
        YACK_GFX_FILTER_DECL(Scharr,3); //!< declare
        YACK_GFX_FILTER_DECL(Scharr,5); //!< declare
#endif

    }
}

#endif
