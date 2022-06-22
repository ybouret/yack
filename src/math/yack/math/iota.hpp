//! \file

#ifndef YACK_IOTA_INCLUDED
#define YACK_IOTA_INCLUDED 1

#include "yack/container/matrix.hpp"

namespace yack {

    namespace math
    {
        //______________________________________________________________________
        //
        //
        //! Inline Operations of Template Algebra
        //
        //______________________________________________________________________
        struct iota
        {

            //------------------------------------------------------------------
            //! target[1..target.size()] = source[1..target.size()]
            //------------------------------------------------------------------
            template <typename TARGET, typename SOURCE> static inline
            void load(TARGET &target, SOURCE &source)
            {
                assert(source.size()>=target.size());
                for(size_t i=target.size();i>0;--i) target[i] = static_cast< typename TARGET::type >( source[i] );
            }

            //------------------------------------------------------------------
            //! target[1..source.size()] = source[1..source.size()]
            //------------------------------------------------------------------
            template <typename TARGET, typename SOURCE> static inline
            void save(TARGET &target, SOURCE &source)
            {
                assert(source.size()<=target.size());
                for(size_t i=source.size();i>0;--i) target[i] = static_cast< typename TARGET::type >( source[i] );
            }


            //! M = A*A'
            template <typename T, typename U> static inline
            void gram(matrix<T> &M, const matrix<U> &A)
            {
                assert(M.rows==A.rows);
                assert(M.is_square());
                const size_t nc = A.cols;
                const size_t nr = M.rows;
                for(size_t i=nr;i>0;--i)
                {
                    matrix_row<T>       &Mi = M[i];
                    const matrix_row<U> &Ai = A[i];
                    for(size_t j=nr;j>0;--j)
                    {
                        const matrix_row<U> &Aj = A[j];
                        T sum = 0;
                        for(size_t k=nc;k>0;--k) sum += Ai[k] * Aj[k];
                        Mi[j] = sum;
                    }
                }
            }

        };

    }

}

#endif

