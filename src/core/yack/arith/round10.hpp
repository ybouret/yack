//! \file

#ifndef YACK_ROUND10_INCLUDED
#define YACK_ROUND10_INCLUDED 1

#include "yack/setup.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {

        //! rounding with powers of ten
        template <typename T>
        struct round10
        {
            //! round using n decimals
            template <unsigned n> static inline
            T floor_with(const T value) noexcept
            {
                static const T _n_(n);
                static const T ten(10);
                const        T p10 = std::floor( std::log10(value) )-_n_;
                return std::floor(value*std::pow(ten,-p10))*pow(ten,p10);
            }
            
        };

    }

}

#endif

