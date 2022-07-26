
//! \file

#ifndef YACK_TYPE_ABS_INCLUDED
#define YACK_TYPE_ANS_INCLUDED 1

#include "yack/setup.hpp"
#include <cmath>

namespace yack
{

    template <typename T>
    struct absolute
    {
        static inline T of(const T x)
        {
            return x < 0 ? -x : x;
        }
    };

    template <>
    struct absolute<float>
    {
        static inline float of(const float x) throw() { return fabsf(x); }
    };

    template <>
    struct absolute<double>
    {
        static inline double of(const double x) throw() { return fabs(x); }
    };


    template <>
    struct absolute<long double>
    {
        static inline double of(const long double x) throw() { return fabsl(x); }
    };
}


#endif
