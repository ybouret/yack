//! \file

#ifndef YACK_NORTH_QSELECT_INCLUDED
#define YACK_NORTH_QSELECT_INCLUDED 1

#include "yack/apex/north/qfamily.hpp"
#include "yack/apex/kernel.hpp"

namespace yack
{
    namespace north
    {
        struct qselect
        {

            template <typename T> static inline
            size_t count_valid(const readable<T> &coef)
            {
                size_t res = 0;
                for(size_t i=coef.size();i>0;--i) {
                    if( 0 != coef[i] ) ++res;
                }
                return res;
            }

            template <typename T> static inline
            size_t compress(matrix<T>       &target,
                          const matrix<T> &source)
            {

                const matrix<T> mu(source,transposed);
                vector<size_t>  jndx(mu.rows,as_capacity);
                {
                    const size_t    m = mu.rows;
                    for(size_t j=1;j<=m;++j)
                    {
                        const readable<T> &mu_j = mu[j];    // select vector
                        if(count_valid(mu_j)<=0) continue;  // drop if null
                        bool original = true;
                        for(size_t k=jndx.size();k>0;--k)
                        {
                            if( apk::are_prop(mu_j,mu[jndx[k]], NULL) )
                            {
                                original = false;
                                break;
                            }
                        }
                        if(!original) continue;             // drop of propto another
                        jndx << j;
                    }
                }
                const size_t  dims = jndx.size();
                if(!dims)
                {
                    return 0;
                }
                else
                {
                    // compress mu
                    const size_t n = mu.cols;
                    target.make(dims,n);
                    for(size_t k=dims;k>0;--k)
                    {
                        writable<T>       &tgt = target[k];
                        const readable<T> &src = mu[ jndx[k] ];
                        for(size_t i=n;i>0;--i) tgt[i] = src[i];
                    }
                    return apk::rank_of(target);
                }
            }
        };

    }

}

#endif
