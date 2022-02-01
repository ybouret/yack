//! \file

#ifndef YACK_MATH_TAO_V2_INCLUDED
#define YACK_MATH_TAO_V2_INCLUDED 1

#include "yack/math/tao/v1.hpp"

namespace yack
{
    namespace math
    {

        namespace tao
        {

            //! matrix/vector
            struct v2
            {
                //! lhs = M*rhs
                template <typename LHS, typename T, typename RHS> static inline
                void mul(LHS &lhs, const matrix<T> &M, RHS &rhs)
                {
                    assert(lhs.size()<=M.rows);
                    for(size_t i=M.rows;i>0;--i)
                    {
                        lhs[i] = v1::dot<typename LHS::mutable_type>::of(M[i],rhs);
                    }
                }
            };

        }

    }

}

#endif
