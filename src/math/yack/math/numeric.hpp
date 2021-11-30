//! \file

#ifndef YACK_MATH_NUMERIC_INCLUDED
#define YACK_MATH_NUMERIC_INCLUDED 1

#include "yack/setup.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {

        inline float        anint(const float       x) throw() { return floor(x+0.5f); } //!< closest float
        inline double       anint(const double      x) throw() { return floor(x+0.5);  } //!< closest double
        inline long double  anint(const long double x) throw() { return floor(x+0.5L); } //!< closest long double


        //! all useful numeric constants
        template <typename T>
        struct numeric
        {
            static const T epsilon;  //!< epsilon
            static const T minimum;  //!< minimum
            static const T maximum;  //!< maximum
            static const T golden_r; //!< (sqrt(5)-1)/2
            static const T golden_c; //!< 1-golden_r
            static const T golden_i; //!< 1/golden_r = 1+golden_r

        };


#if !defined(_MSC_VER)
        //! declaring constants is necessary
#define YACK_MATH_NUMERIC(VALUE)                           \
template <> const float       numeric<float>      ::VALUE; \
template <> const double      numeric<double>     ::VALUE; \
template <> const long double numeric<long double>::VALUE

        YACK_MATH_NUMERIC(epsilon);  //!< epsilon
        YACK_MATH_NUMERIC(minimum);  //!< minimum
        YACK_MATH_NUMERIC(maximum);  //!< maximum
        YACK_MATH_NUMERIC(golden_r); //!< golden_r
        YACK_MATH_NUMERIC(golden_c); //!< golden_c
        YACK_MATH_NUMERIC(golden_i); //!< golden_i


#endif


        template <typename T>
        static inline bool almost_equal( const T X, const T Y) throw()
        {
            static T fac = T(0.5) * numeric<T>::epsilon;
            return ( fabs(X-Y) <= fac * ( fabs(X) + fabs(Y) ) );
        }

    }

}

#endif
