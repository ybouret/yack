//! \file

#ifndef YACK_MATH_NUMERIC_INCLUDED
#define YACK_MATH_NUMERIC_INCLUDED 1

#include "yack/type/utils.hpp"
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
            static const T        epsilon;     //!< epsilon
            static const T        minimum;     //!< minimum
            static const T        maximum;     //!< maximum
            static const T        golden_r;    //!< (sqrt(5)-1)/2
            static const T        golden_c;    //!< 1-golden_r
            static const T        golden_i;    //!< 1/golden_r = 1+golden_r
            static const unsigned mant_dig;    //!< mantissa digits
            static const unsigned dig;         //!< digital decimal precision
            static const T        ftol;        //!< 10^(-dig)
            static const int      min_exp;     //!< minimal exp value
            static const int      max_exp;     //!< maximal exp value
            static const int      min_10_exp;  //!< minimal 10 exp value
            static const int      max_10_exp;  //!< maximal 10 exp value
        };


#if !defined(_MSC_VER)
        //! declaring constants is necessary
#define YACK_MATH_NUMERIC(VALUE)                           \
template <> const float       numeric<float>      ::VALUE; \
template <> const double      numeric<double>     ::VALUE; \
template <> const long double numeric<long double>::VALUE

        YACK_MATH_NUMERIC(epsilon);     //!< epsilon
        YACK_MATH_NUMERIC(minimum);     //!< minimum
        YACK_MATH_NUMERIC(maximum);     //!< maximum
        YACK_MATH_NUMERIC(golden_r);    //!< golden_r
        YACK_MATH_NUMERIC(golden_c);    //!< golden_c
        YACK_MATH_NUMERIC(golden_i);    //!< golden_i
        YACK_MATH_NUMERIC(ftol);        //!< ftol

#endif


        //! test almost equal
        template <typename T>
        inline bool almost_equal( const T X, const T Y) throw()
        {
            static T fac = T(0.5) * numeric<T>::epsilon;
            return ( fabs(X-Y) <= fac * ( fabs(X) + fabs(Y) ) );
        }

        //! precise hypothenuse
        template <typename T>
        inline T hypothenuse(const T a, const T b) throw()
        {
            static const T one(1);
            const T        absa=fabs(a);
            const T        absb=fabs(b);
            if(absa>absb)
            {
                return absa*sqrt(one+squared(absb/absa));
            }
            else
            {
                return (absb <= 0 ? 0 : absb*sqrt(one+squared(absa/absb)));
            }
        }

        //! return the signed value of a w.r.t the sign of b
        template <typename T, typename U>
        inline T __sgn(T a, U b) throw()
        {
            return (b >= 0) ? fabs(a) : -fabs(a);
        }


    }

}

#endif
