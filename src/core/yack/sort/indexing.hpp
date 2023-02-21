
//! \file
#ifndef YACK_SORT_INDEXING_INCLUDED
#define YACK_SORT_INDEXING_INCLUDED 1

#include "yack/container/writable.hpp"
#include "yack/comparison.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! indexing functions
    //
    //__________________________________________________________________________
    struct indexing
    {
        //! make index, C++ style
        template <typename T,typename FUNC> static inline
        void make(writable<size_t> &idx, FUNC &compare, const readable<T> &ra) noexcept
        {

            // initialize indices
            const size_t n = idx.size(); assert( ra.size() == idx.size() );
            for(size_t i=n;i>0;--i) idx[i] = i;

            // sort routine
            size_t   inc = 1;
            do
            {
                inc *=3;
                ++inc;
            }
            while( inc <= n );
            do
            {
                inc /= 3;
                for( size_t i=inc+1;i<=n;++i)
                {
                    const size_t   t = idx[i];
                    const T       &v = ra[t];
                    size_t         j = i;
                    size_t         d = j-inc;
                    assert( t      <= n );
                    assert( idx[d] <= n );
                    while( compare( v, ra[ idx[d] ] ) < 0 )
                    {
                        idx[j] = idx[d];
                        j = d;
                        if( j <= inc )
                            break;
                        d -= inc;
                    }
                    idx[ j ] = t;
                }

            }
            while( inc > 1 );
        }

        //! rank source into target according to indices
        template <typename T> static inline
        void rank( writable<T> &target, const readable<T> &source, const readable<size_t> &indices )
        {
            assert(target.size()==source.size());
            assert(target.size()==indices.size());
            for(size_t i=target.size();i>0;--i)
            {
                assert(indices[i]>0);
                assert(indices[i]<=target.size());
                target[i] = source[indices[i]];
            }
        }





    };

}

#endif
