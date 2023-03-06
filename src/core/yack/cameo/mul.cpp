#include "yack/cameo/mul.hpp"
#include "yack/type/complex.hpp"
#include <cmath>

namespace yack
{
    namespace cameo
    {
        template <typename T>
        int exponent_for(const T &) noexcept;

        template <>
        int exponent_for<float>(const float &f) noexcept {
            int ex = 0;
            (void) std::frexp(f,&ex);
            return ex;
        }

        template <>
        int exponent_for<double>(const double &f) noexcept {
            int ex = 0;
            (void) std::frexp(f,&ex);
            return ex;
        }

        template <>
        int exponent_for<long double>(const long double &f) noexcept {
            int ex = 0;
            (void) std::frexp(f,&ex);
            return ex;
        }

        template <>
        int exponent_for< complex<float> >(const complex<float> &f) noexcept {
            int ex = 0;
            (void) std::frexp(max_of( std::fabs(f.re), std::fabs(f.im) ),&ex);
            return ex;
        }

        template <>
        int exponent_for< complex<double> >(const complex<double> &f) noexcept {
            int ex = 0;
            (void) std::frexp(max_of( std::fabs(f.re), std::fabs(f.im) ),&ex);
            return ex;
        }

        template <>
        int exponent_for< complex<long double> >(const complex<long double> &f) noexcept {
            int ex = 0;
            (void) std::frexp(max_of( std::fabs(f.re), std::fabs(f.im) ),&ex);
            return ex;
        }

    }
}

#define real_t float
#include "mul.hxx"


#undef  real_t
#define real_t double
#include "mul.hxx"

#undef  real_t
#define real_t long double
#include "mul.hxx"


#undef  real_t
#define real_t complex<float>
#include "mul.hxx"

#undef  real_t
#define real_t complex<double>
#include "mul.hxx"

#undef  real_t
#define real_t complex<long double>
#include "mul.hxx"
