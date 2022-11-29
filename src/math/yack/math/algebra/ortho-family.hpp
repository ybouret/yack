//! \file

#ifndef YACK_MATH_ORTHO_FAMILY_INCLUDED
#define YACK_MATH_ORTHO_FAMILY_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace math
    {
        
        struct ortho_family
        {
            
            static bool make(matrix<apq> &Q, const matrix<apq> &P);
            
            template <typename T> static inline
            bool build(matrix<apq> &Q, const matrix<T> &J)
            {
                const matrix<apq> P(J,transmogrify);
                return make(Q,P);
            }
            
        };
        
    }
}

#endif

