#include "yack/apex/kernel.hpp"

namespace yack
{

    apn apk:: lcm(const matrix<apq> &M)
    {
        if(M.rows)
        {
            return lcm<matrix<apq>,size_t>(M,1,M.cols,1,M.rows);
        }
        else
        {
            return 1;
        }
    }


    static inline
    void apk_simplify(writable<apq> &Q)
    {
        //----------------------------------------------------------------------
        // find first positive value
        //----------------------------------------------------------------------
        apn          g = 1;
        const size_t n = Q.size();
        size_t       j = 1;
        for(;j<=n;++j)
        {
            const apq &q = Q[j];
            if(q!=0) {
                g = q.num.n;
                break;
            }
        }

        //----------------------------------------------------------------------
        // compute with other positive values
        //----------------------------------------------------------------------
        for(++j;j<=n;++j)
        {
            const apq &q = Q[j];
            if(q!=0) {
                g = apn::gcd(q.num.n,g);
            }
        }

        //----------------------------------------------------------------------
        // simplify
        //----------------------------------------------------------------------
        for(size_t i=n;i>0;--i)
        {
            Q[i] /= g;
            assert(1==Q[i].den);
        }

    }

    void apk:: simplify(writable<apq> &Q)
    {
        const size_t n = Q.size();
        switch(n)
        {
                //...
            case 0:
                return;

                // keep only numerator
            case 1: {
                apq &q = Q[1];
                const apz z = q.num;

            } return;

                // make all denominators as 1
            default: {
                const apq _ = lcm(Q,size_t(1),n);
                for(size_t i=n;i>0;--i) { Q[i] *= _; assert(1==Q[i].den); }
            } break;
        }
        assert(n>=2);

        apk_simplify(Q);
    }


    void apk:: simplify(writable<apz> &z)
    {
        const size_t n = z.size();
        switch(n)
        {
            case 0: return;
            case 1:
                switch(z[1].s)
                {
                    case __zero__:       break;
                    case positive: z[1]= 1; break;
                    case negative: z[1]=-1; break;
                }
                return;
            default:
                break;
        }

        //----------------------------------------------------------------------
        // find first positive value
        //----------------------------------------------------------------------
        apn          g = 1;
        size_t       j = 1;
        for(;j<=n;++j)
        {
            const apz &q = z[j];
            if(q!=0) {
                g = q.n;
                break;
            }
        }

        //----------------------------------------------------------------------
        // compute with other positive values
        //----------------------------------------------------------------------
        for(++j;j<=n;++j)
        {
            const apz &q = z[j];
            if(q!=0) {
                g = apn::gcd(q.n,g);
            }
        }

        //----------------------------------------------------------------------
        // simplify
        //----------------------------------------------------------------------
        for(size_t i=n;i>0;--i)
        {
            z[i] /= g;
        }

    }
    
    
    void apk:: simplify(writable<apn> &z)
    {
        const size_t n = z.size();
        switch(n)
        {
            case 0: return;
            case 1:
                if(z[1]>0) z[1] = 1;
                return;
            default:
                break;
        }
        
        //----------------------------------------------------------------------
        // find first positive value
        //----------------------------------------------------------------------
        apn          g = 1;
        size_t       j = 1;
        for(;j<=n;++j)
        {
            const apn &q = z[j];
            if(q!=0) {
                g = q;
                break;
            }
        }
        
        //----------------------------------------------------------------------
        // compute with other positive values
        //----------------------------------------------------------------------
        for(++j;j<=n;++j)
        {
            const apn &q = z[j];
            if(q!=0) {
                g = apn::gcd(q,g);
            }
        }
        
        //----------------------------------------------------------------------
        // simplify
        //----------------------------------------------------------------------
        for(size_t i=n;i>0;--i)
        {
            z[i] /= g;
        }
        
    }
}

#include "yack/math/api.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    using namespace math;


    size_t apk:: rank(matrix<apq> &Q)
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


    static inline apq apq_dot(const readable<apq> &lhs,
                              const readable<apq> &rhs)
    {
        apq sum(0);
        for(size_t i=lhs.size();i>0;--i) sum += lhs[i] * rhs[i];
        return sum;
    }

    static inline apq apq_norm2(const readable<apq> &lhs)
    {
        apq sum(0);
        for(size_t i=lhs.size();i>0;--i) sum += lhs[i]*lhs[i];
        return sum;
    }


    bool apk:: gs_ortho(matrix<apq> &V)
    {
        const size_t n = V.rows;
        const size_t m = V.cols;
        matrix<apq>  U(n,m);
        {
            vector<apq>  u2(n);

            for(size_t k=1;k<=n;++k)
            {
                const readable<apq>     &v_k = V[k];
                writable<apq>           &u_k = U[k];
                for(size_t i=m;i>0;--i)  u_k[i] = v_k[i];

                for(size_t j=1;j<k;++j)
                {
                    assert(u2[j]>0);
                    assert(apq_norm2(U[j])==u2[j]);
                    const readable<apq> &u_j = U[j];
                    const apq            cof = apq_dot(u_j,v_k)/u2[j];
                    for(size_t i=m;i>0;--i)
                    {
                        u_k[i] -= cof * u_j[i];
                    }
                }
                apk::simplify(u_k);
                if( (u2[k] = apq_norm2(u_k)) <= 0) return false;

                size_t    np = 0;        // number of positive
                size_t    nn = 0;        // number of negative
                sign_type fs = __zero__; // first sign

                for(size_t i=1;i<=m;++i)
                {
                    switch(  u_k[i].num.s )
                    {
                        case __zero__:
                            break;

                        case positive:
                            ++np;
                            if(__zero__==fs)
                            {
                                assert(nn<=0);
                                assert(1==np);
                                fs=positive;
                            }
                            break;

                        case negative:
                            ++nn;
                            if(__zero__==fs)
                            {
                                assert(np<=0);
                                assert(1==nn);
                                fs=negative;
                            }
                            break;
                    }
                }

                assert(nn>0||np>0);
                assert(__zero__!=fs);
                //std::cerr << "u_k=" << u_k << " : " << np << "+, " << nn << "-" << ", fs=" << fs << std::endl;
                if(nn>np || (np>=nn && negative==fs) )
                {
                    for(size_t i=m;i>0;--i)
                    {
                        u_k[i] = -u_k[i];
                    }
                }

                
            }
        }
        V.assign(U);


        return true;
    }


}
