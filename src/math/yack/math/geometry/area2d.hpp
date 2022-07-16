//! \file

#ifndef YACK_MATH_GEOMETRY_AREA2D_INCLUDED
#define YACK_MATH_GEOMETRY_AREA2D_INCLUDED 1


#include "yack/setup.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {
        
        struct area2d
        {
            template <typename LHS, typename RHS> static inline
            typename LHS::mutable_type
            triangle(LHS &x, RHS &y) throw()
            {
                static typename LHS::const_type half(0.5);
                assert(x.size()>=3);
                assert(y.size()>=3);
                typename LHS::const_type xab = x[2]-x[1];
                typename LHS::const_type yab = y[2]-y[1];
                typename LHS::const_type xac = x[3]-x[1];
                typename LHS::const_type yac = y[3]-y[1];
                return half * std::abs( xab*yac - xac*yab);
            }
            
        };
    }
}

#endif

