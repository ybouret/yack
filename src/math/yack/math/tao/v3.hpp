
//! \file

#ifndef YACK_MATH_TAO_V3_INCLUDED
#define YACK_MATH_TAO_V3_INCLUDED 1

#include "yack/math/tao/v1.hpp"

namespace yack
{
    namespace math
    {

        namespace tao
        {

            struct v3
            {
                template <typename T, typename ARRAY, typename U> static inline
                void dmul(matrix<T> &M, ARRAY &d, const matrix<U> &rhs)
                {
                    assert(M.is_square());
                    assert(matrix_metrics::have_same_sizes(M,rhs));
                    const size_t n = M.rows;
                    for(size_t i=n;i>0;--i)
                    {
                        const T di = T(d[i]);
                        v1::mul(M[i],di,rhs[i]);
                    }
                }


                template <typename T, typename U, typename V> static inline
                void mmul(matrix<T> &M, const matrix<U> &A, const matrix<V> &B)
                {
                    assert(M.rows==A.rows);
                    assert(M.cols==B.cols);
                    assert(A.cols==B.rows);
                    const size_t nk = A.cols;
                    const size_t nc = M.cols;
                    for(size_t i=M.rows;i>0;--i)
                    {
                        matrix_row<T>       &Mi = M[i];
                        const matrix_row<T> &Ai = A[i];
                        for(size_t j=nc;j>0;--j)
                        {
                            T sum = 0;
                            for(size_t k=nk;k>0;--k) sum += Ai[k] * B[k][j];
                            Mi[j] = sum;
                        }
                    }
                }

            };

        }

    }

}

#endif
