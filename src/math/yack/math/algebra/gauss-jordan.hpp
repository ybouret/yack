//! \file

#ifndef YACK_MATH_GAUSSJ_INCLUDED
#define YACK_MATH_GAUSSJ_INCLUDED 1

#include "yack/container/matrix.hpp"
#include "yack/math/api.hpp"

namespace yack
{
    namespace math
    {
        //! Gauss Jordan methods
        template <typename T> struct gauss_jordan
        {
            YACK_DECL_ARGS_(T,type);                             //!< aliases
            typedef typename scalar_for<type>::type scalar_type; //!< alias

            //! build forward
            static inline
            bool build(matrix<T> &a, writable<T> &b)
            {
                assert( a.is_square() );
                assert( b.size() >= a.rows );
                const size_t n = a.rows;
                

                return true;
            }

        };
    }
}

#endif


