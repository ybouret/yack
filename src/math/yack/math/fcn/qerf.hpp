//! \file

#ifndef YACK_MATH_QERF_INCLUDED
#define YACK_MATH_QERF_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace math
    {

        template <typename T> T qerfc(const T x) noexcept; //!< quick error function complement
        template <typename T> T qerf(const T x)  noexcept; //!< quick error function

    }

}

#endif

