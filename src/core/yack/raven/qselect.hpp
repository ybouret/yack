//! \file

#ifndef YACK_RAVEN_QSELECT_INCLUDED
#define YACK_RAVEN_QSELECT_INCLUDED 1

#include "yack/apex/kernel.hpp"

namespace yack
{
    namespace raven
    {
        //______________________________________________________________________
        //
        //
        //! selecting procedures
        //
        //______________________________________________________________________
        struct qselect
        {

            //__________________________________________________________________
            //
            //! counting coefficients different of zero
            //__________________________________________________________________
            template <typename T> static inline
            size_t count_valid(const readable<T> &coef)
            {
                size_t res = 0;
                for(size_t i=coef.size();i>0;--i) {
                    if( 0 != coef[i] ) ++res;
                }
                return res;
            }

            //__________________________________________________________________
            //
            //! compressing matrix with non zero, non proportional rows
            /**
             \param rvec row vectors
             \param topo initial topology
             */
            //__________________________________________________________________
            template <typename T> static inline
            size_t compress(matrix<T>       &rvec,
                            const matrix<T> &topo)
            {
                const matrix<T> mu(topo,transposed);        // transposed
                vector<size_t>  jndx(mu.rows,as_capacity);  // valid indices
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
                    const size_t n = mu.cols;
                    rvec.make(dims,n);
                    for(size_t k=dims;k>0;--k)
                    {
                        writable<T>       &tgt = rvec[k];
                        const readable<T> &src = mu[ jndx[k] ];
                        for(size_t i=n;i>0;--i) tgt[i] = src[i];
                    }
                    return apk::rank_of(rvec);
                }
            }
        };

    }

}

#endif
