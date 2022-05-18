//! \file

#ifndef YACK_COLOR_OPAQUE_INCLUDED
#define YACK_COLOR_OPAQUE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace color
    {
        //______________________________________________________________________
        //
        //! generic opaque field
        //______________________________________________________________________
        template <typename T>
        struct opaque
        {
            static const T value; //!< 0xff or 1
        };

#if !defined(_MSC_VER)
        template <> const float       opaque<float>::       value; //!< forward
        template <> const double      opaque<double>::      value; //!< forward
        template <> const long double opaque<long double>:: value; //!< forward
        template <> const uint8_t     opaque<uint8_t>::     value; //!< forward
#endif

    }
}

#endif

