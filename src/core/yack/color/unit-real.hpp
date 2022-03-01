//! \file

#ifndef YACK_COLOR_UNIT_REAL_INCLUDED
#define YACK_COLOR_UNIT_REAL_INCLUDED 1

#include "yack/setup.hpp"
#include <cmath>

namespace yack
{
    namespace color
    {
        //______________________________________________________________________
        //
        //! unit reals to/from bytes
        //______________________________________________________________________
        template <typename T>
        struct unit_real
        {
            static const T table[256];                 //!< [0..255]/255

            //! [0..1] -> [0..255]
            static inline uint8_t to_byte(const T x) throw()
            {
                static const T word(255);
                static const T half(0.5);
                return T( floor(word*x+half) );
            }
        };

#if !defined(_MSC_VER)
        template <> const float       unit_real<float>::       table[]; //!< decl
        template <> const double      unit_real<double>::      table[]; //!< decl
        template <> const long double unit_real<long double>:: table[]; //!< decl
#endif

    }
}

#endif

