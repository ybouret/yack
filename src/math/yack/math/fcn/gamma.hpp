
//! \file

#ifndef YACK_MATH_GAMMA_INCLUDED
#define YACK_MATH_GAMMA_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace math
    {

        template <typename T> T gammaln(const T x);         //!< ln(Gamma(x))
        template <typename T> T beta(const T w, const T z); //!< Gamma(w)*Gamma(z)/Gamma(w+z)
        template <typename T> T factln(const int n);        //!< ln(n!)
    }

}

#endif

