//! \file

#ifndef YACK_WOVEN_QCOMPRESS_INCLUDED
#define YACK_WOVEN_QCOMPRESS_INCLUDED 1

#include "yack/apex/flak.hpp"
#include "yack/data/dinky/core-repo.hpp"

namespace yack
{
    namespace woven
    {

        struct qcompress
        {

            template <typename ARRAY>
            static inline size_t neqz(ARRAY &arr)
            {
                size_t res=0;
                for(size_t i=arr.size();i>0;--i)
                {
                    if(0!=arr[i]) ++res;
                }
                return res;
            }

            template <typename T, typename U> static inline
            size_t build(matrix<T>       &Qc,
                         const matrix<U> &Q,
                         size_t           nmin=0)
            {
                typedef readable<U>                  row_type;
                typedef core_repo<row_type>          row_repo;
                typedef typename row_repo::node_type row_node;

                nmin = max_of<size_t>(nmin,1);
                row_repo rows;
                for(size_t i=1;i<=Q.rows;++i)
                {
                    const row_type &rhs  = Q[i];
                    if( neqz(rhs) < nmin ) continue;;

                    bool ok  = true;
                    for(const row_node *node=rows.head;node;node=node->next)
                    {
                        const row_type &lhs = ***node;
                        if(apex::flak::colinear(lhs,rhs))
                        {
                            ok = false;
                            break;
                        }
                    }
                    if(ok) rows << rhs;
                }
                const size_t nr = rows.size;
                if(nr<=0)
                {
                    Qc.release();
                    return 0;
                }
                else
                {
                    const size_t nc = Q.cols;
                    Qc.make(nr,nc);
                    size_t ir=1;
                    for(const row_node *node=rows.head;node;node=node->next,++ir)
                    {
                        writable<T>    &target = Qc[ir];
                        const row_type &source = ***node; assert(source.size()==target.size());
                        for(size_t j=nc;j>0;--j) target[j] = static_cast<T>(source[j]);
                    }
                    return apex::flak::rank(Qc);
                }
            }


        };
    }
}

#endif

