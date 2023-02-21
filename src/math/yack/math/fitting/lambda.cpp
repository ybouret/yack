
#include "yack/math/fitting/lambda.hpp"
#include <cfloat>
#include <cmath>

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            template <> const char * lambda<float>::clid()  noexcept
            {
                return "lambda<float>";
            }

            template <>
            int lambda<float>::pmin() noexcept
            {
                static const int _ = int( ceilf( log10f(FLT_EPSILON)));
                return _;
            }

            template <>
            int lambda<float>:: pmax() noexcept
            {
                static const int _ = int(FLT_MAX_10_EXP);
                return _;
            }

            template <>
            int lambda<float>:: ptol() noexcept
            {
                static const int _ =  int( floorf(log10f(sqrtf(FLT_EPSILON))) );
                return _;
            }

#define real_t float
#include "lambda.hxx"

            template <> const char * lambda<double>::clid()  noexcept
            {
                return "lambda<double>";
            }

            template <>
            int lambda<double>::pmin() noexcept
            {
                static const int _ = int( ceil( log10(DBL_EPSILON)));
                return _;
            }

            template <>
            int lambda<double>:: pmax() noexcept
            {
                static const int _ = int(DBL_MAX_10_EXP);
                return _;
            }

            template <>
            int lambda<double>:: ptol() noexcept
            {
                static const int _ =  int( floor(log10(sqrt(DBL_EPSILON))) );
                return _;
            }

#undef  real_t
#define real_t double
#include "lambda.hxx"


            template <> const char * lambda<long double>::clid()  noexcept
            {
                return "lambda<long double>";
            }

            template <>
            int lambda<long double>::pmin() noexcept
            {
                static const int _ = int( ceill( log10l(LDBL_EPSILON)));
                return _;
            }

            template <>
            int lambda<long double>:: pmax() noexcept
            {
                static const int _ = int(LDBL_MAX_10_EXP);
                return _;
            }

            template <>
            int lambda<long double>:: ptol() noexcept
            {
                static const int _ =  int(floorl(log10l(sqrtl(LDBL_EPSILON))));
                return _;
            }

#undef  real_t
#define real_t long double
#include "lambda.hxx"



        }

    }
}
