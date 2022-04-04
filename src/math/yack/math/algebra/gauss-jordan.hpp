//! \file

#ifndef YACK_MATH_GAUSSJ_INCLUDED
#define YACK_MATH_GAUSSJ_INCLUDED 1

#include "yack/container/matrix.hpp"
#include "yack/math/api.hpp"

namespace yack
{
    namespace math
    {
        template <typename T> struct gauss_jordan
        {
            YACK_DECL_ARGS_(T,type);
            typedef typename scalar_for<type>::type scalar_type;

            static inline
            bool build(matrix<T> &a, writable<T> &b)
            {
                assert( a.is_square() );
                assert( b.size() >= a.rows );
                const size_t n = a.rows;
                for(size_t im=1,i=2;i<=n;++i,++im)
                {
                    const T &piv = a[im][im];
                    {
                        const scalar_type apiv = mod2_of(piv);
                        if(apiv<=0) return false;
                    }
                    for(size_t k=i;k<=n;++k)
                    {
                        const_type coef = a[k][im] / piv;
                        a[k][im] = 0;
                        for(size_t j=n;j>k;--j) a[k][j] -= coef * a[im][k];
                        b[k] -= coef * b[im];
                    }
                }

                return true;
            }

        };
    }
}

#endif


