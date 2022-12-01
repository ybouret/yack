//! \file

#ifndef YACK_MATH_ORTHO_FAMILY_INCLUDED
#define YACK_MATH_ORTHO_FAMILY_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace math
    {

        //! computing orthogonal family for a set of independent vector
        struct ortho_family
        {

            //! Q.rows = Q.cols = P.cols
            static bool make(matrix<apq> &Q, const matrix<apq> &P);

            //! copy/make
            template <typename T> static inline
            bool build(matrix<apq> &Q, const matrix<T> &J)
            {
                const  matrix<apq> P(J,transmogrify);
                return make(Q,P);
            }
            
        };
        
    }
}

#endif

