
//! \file

#ifndef YACK_TYPE_ABS_INCLUDED
#define YACK_TYPE_ABS_INCLUDED 1

#include "yack/setup.hpp"
#include <cmath>

namespace yack
{

    namespace low_level
    {
        //! generic absolute value for SCALAR values
        template <typename T>
        struct absolute
        {
            //! get
            static inline T of(const T x)
            {
                return x < 0 ? -x : x;
            }
        };

        //! specific absolute value
        template <>
        struct absolute<float>
        {
            //! get
            static inline float of(const float x) throw() { return fabsf(x); }
        };

        //! specific absolute value
        template <>
        struct absolute<double>
        {
            //! get
            static inline double of(const double x) throw() { return fabs(x); }
        };

        //! specific absolute value
        template <>
        struct absolute<long double>
        {
            //! get
            static inline double of(const long double x) throw() { return fabsl(x); }
        };
    }

    //! wrapper for absolute value
    template <typename T>
    inline T absolute(const T &x)
    {
        return low_level::absolute<T>::of(x);
    }

}


#endif
