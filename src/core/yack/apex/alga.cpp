
#include "yack/apex/alga.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{

    bool alga:: colinear(const readable<int> &lhs, const readable<int> &rhs)
    {
        assert( lhs.size() == rhs.size() );
        auto_ptr<const apq> f = NULL;

        for(size_t i=lhs.size();i>0;--i)
        {
            const int l = lhs[i];
            const int r = rhs[i];
            switch( __sign::pair_of(l,r) )
            {
                case zz_pair: continue; // compatible;
                case zp_pair:
                case pz_pair:
                case zn_pair:
                case nz_pair:
                    return false; // incompatible

                case nn_pair:
                case pp_pair: {
                    const apq rho(absolute(l),absolute(r));
                    if(f.is_empty())
                    {
                        f = new apq(rho);
                    }
                    else
                    {
                        if( *f != rho ) return false;
                    }
                } continue;

                case np_pair:
                case pn_pair: {
                    const apq rho(absolute(l),absolute(r)); coerce(rho.num.s) = negative;
                    if(f.is_empty())
                    {
                        f = new apq(rho);
                    }
                    else
                    {
                        if( *f != rho ) return false;
                    }
                } continue;
            }
        }

        return true;
    }

}
#include "yack/math/api.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    using namespace math;


    size_t alga:: rank_of(matrix<apq> &Q)
    {
        const size_t m    = Q.rows; // |rows|
        const size_t n    = Q.cols; // |cols|
        size_t       h    = 1;      // pivot row
        size_t       k    = 1;      // pivot col
        size_t       rank = 0;      // current ranl
        const apq    _0   = 0;      // helper

        //______________________________________________________________________
        //
        // Running on rows and columns
        //______________________________________________________________________
        while(h<=m && k<=n)
        {
            //__________________________________________________________________
            //
            // initialize @row #h the search for pivot @column #k
            //__________________________________________________________________
            readable<apq> &Qh = Q[h];
            size_t i_max = h;
            apq    q_piv = Qh[k];
            apq    a_max = abs_of(q_piv);

            //__________________________________________________________________
            //
            // look for pivot in column k
            //__________________________________________________________________
            for(size_t i=h+1;i<=m;++i) {
                const apq &q_tmp = Q[i][k];
                const apq  a_tmp = abs_of(q_tmp);
                if(a_tmp>a_max) {
                    q_piv = q_tmp;
                    a_max = a_tmp;
                    i_max = i;
                }
            }

            if(a_max<=0) {
                ++k; // pass to next column
            }
            else {
                assert(a_max>0);
                assert(q_piv!=0);

                ++rank;                // found a valid rank
                Q.swap_rows(h,i_max);  // at i_max
                assert(q_piv==Qh[k]);
                for(size_t i=h+1;i<=m;++i)
                {
                    assert(Q[h][k]!=0);
                    writable<apq> &Qi = Q[i];
                    const apq      f  = Qi[k]/q_piv;
                    Q[i][k]     = _0;
                    for(size_t j = k+1; j<=n; ++j)
                    {
                        Qi[j] -= Qh[j] * f;
                    }
                }
                ++h; // pass to next line
                ++k; // pass to next column
            }
        }

        return rank;
    }
}

