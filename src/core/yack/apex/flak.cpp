
#include "yack/apex/flak.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/api.hpp"

namespace yack
{

    namespace apex
    {

        void flak:: raise_error(const char *msg)
        {
            assert(msg);
            throw imported::exception("apex::flak","%s",msg);
        }

        void flak:: simplify(writable<apq> &q)
        {
            const apn fac = common_denominator(q);
            for(size_t i=q.size();i>0;--i)
            {
                q[i] *= fac;
            }
        }

        void flak:: univocal( writable<apq> &v )
        {
            //------------------------------------------------------------------
            //
            // process trivial cases
            //
            //------------------------------------------------------------------
            const size_t n = v.size();
            switch(n)
            {
                case 0: return;
                case 1: {
                    apq &q = v[1];
                    switch(q.num.s)
                    {
                        case __zero__: return;
                        case positive:
                        case negative: q = 1; return;
                    }
                } return;

                default:
                    break;
            }

            //------------------------------------------------------------------
            //
            // generic case
            //
            //------------------------------------------------------------------
            assert(n>=2);
            sign_type fs = __zero__;  // first sign
            size_t    np = 0;         // number of positive
            size_t    nn = 0;         // number of negative
            apn       g  = 1;         // divider
            {
                //--------------------------------------------------------------
                // compute common denominator
                //--------------------------------------------------------------
                const apn m  = common_denominator(v);

                //--------------------------------------------------------------
                // simplify up to fist sign
                //--------------------------------------------------------------
                size_t    i  = 1;
                for(;i<=n;++i)
                {
                    apq &q = v[i];
                    switch(q.num.s)
                    {
                        case __zero__: continue;
                        case negative: ++nn; fs=negative; break;
                        case positive: ++np; fs=positive; break;
                    }
                    assert(__zero__!=fs);
                    q *= m; assert(1==q.den);
                    g  = q.num.n;
                    break;
                }

                //--------------------------------------------------------------
                // propagate
                //--------------------------------------------------------------
                for(++i;i<=n;++i)
                {
                    apq &q = v[i];
                    switch(q.num.s)
                    {
                        case __zero__: continue;
                        case negative: ++nn; break;
                        case positive: ++np; break;
                    }

                    q *= m;   assert(1==q.den);
                    g  = apn::gcd(q.num.n,g);
                }
            }

            //------------------------------------------------------------------
            //
            // analyze number of signs
            //
            //------------------------------------------------------------------
            if(nn>np || (nn==np&&negative==fs))
            {
                //--------------------------------------------------------------
                // changing signs
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i)
                {
                    const apz &z = v[i].num;
                    sign_type &s = coerce(z.s);
                    switch(s)
                    {
                        case __zero__: continue;
                        case positive: s=negative; break;
                        case negative: s=positive; break;
                    }
                    coerce(z.n) /= g;
                }
            }
            else
            {
                //--------------------------------------------------------------
                // keeping signs
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i)
                    coerce(v[i].num.n) /= g;
            }
        }

        size_t flak:: rank_of(matrix<apq> &Q)
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
                apq    a_max = math::abs_of(q_piv);

                //__________________________________________________________________
                //
                // look for pivot in column k
                //__________________________________________________________________
                for(size_t i=h+1;i<=m;++i) {
                    const apq &q_tmp = Q[i][k];
                    const apq  a_tmp = math::abs_of(q_tmp);
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
}
