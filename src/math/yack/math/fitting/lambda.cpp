
#include "yack/math/fitting/lambda.hpp"
#include <cfloat>
#include <cmath>

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            template <> const char * lambda<float>::clid()  throw()
            {
                return "lambda<float>";
            }

            template <>
            int lambda<float>::pmin() throw()
            {
                static const int _ = int( ceilf( log10f(FLT_EPSILON)));
                return _;
            }

            template <>
            int lambda<float>:: pmax() throw()
            {
                static const int _ = int(FLT_MAX_10_EXP);
                return _;
            }

            template <>
            int lambda<float>:: ptol() throw()
            {
                static const int _ =  int( floorf(log10f(sqrtf(FLT_EPSILON))) );
                return _;
            }

#define real_t float
#include "lambda.hxx"

            template <> const char * lambda<double>::clid()  throw()
            {
                return "lambda<double>";
            }

            template <>
            int lambda<double>::pmin() throw()
            {
                static const int _ = int( ceil( log10(DBL_EPSILON)));
                return _;
            }

            template <>
            int lambda<double>:: pmax() throw()
            {
                static const int _ = int(DBL_MAX_10_EXP);
                return _;
            }

            template <>
            int lambda<double>:: ptol() throw()
            {
                static const int _ =  int( floor(log10(sqrt(DBL_EPSILON))) );
                return _;
            }

#undef  real_t
#define real_t double
#include "lambda.hxx"


            template <> const char * lambda<long double>::clid()  throw()
            {
                return "lambda<long double>";
            }

            template <>
            int lambda<long double>::pmin() throw()
            {
                static const int _ = int( ceill( log10l(LDBL_EPSILON)));
                return _;
            }

            template <>
            int lambda<long double>:: pmax() throw()
            {
                static const int _ = int(LDBL_MAX_10_EXP);
                return _;
            }

            template <>
            int lambda<long double>:: ptol() throw()
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
