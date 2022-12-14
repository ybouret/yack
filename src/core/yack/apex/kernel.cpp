#include "yack/apex/kernel.hpp"


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


    static inline sign_type  first_sign(const readable<apq> &v) throw()
    {
        const size_t n = v.size();
        for(size_t i=1;i<=n;++i)
        {
            const sign_type s = v[i].num.s;
            if(s!=__zero__) return s;
        }
        return __zero__;
    }

    static inline void       change_signs(writable<apq> &v) throw() {
        for(size_t i=v.size();i>0;--i)
        {
            sign_type &s = coerce(v[i].num.s);
            s = __sign::opposite(s);
        }
    }

    void apk:: univocal(writable<apq> &v) throw()
    {
        assert(v.size()>0);
        size_t    np = 0;        // number of positive
        size_t    nn = 0;        // number of negative
        for(size_t i=v.size();i>0;--i)
        {
            switch(  v[i].num.s )
            {
                case __zero__:
                    break;

                case positive:
                    ++np;
                    break;

                case negative:
                    ++nn;

                    break;
            }
        }

        assert(nn>0||np>0);
        switch( __sign::of(np,nn) )
        {
            case positive: // do nothing
                assert(np>nn);
                break;

            case negative: // change signs
                assert(np<nn);
                change_signs(v);
                break;

            case __zero__: // change only if first sign is negative
                assert(nn==np);
                if(negative==first_sign(v)) change_signs(v);
                break;
        }
    }


    static inline sign_type  first_sign(const readable<apz> &v) throw()
    {
        const size_t n = v.size();
        for(size_t i=1;i<=n;++i)
        {
            const sign_type s = v[i].s;
            if(s!=__zero__) return s;
        }
        return __zero__;
    }

    static inline void       change_signs(writable<apz> &v) throw() {
        for(size_t i=v.size();i>0;--i)
        {
            sign_type &s = coerce(v[i].s);
            s = __sign::opposite(s);
        }
    }

    void apk:: univocal(writable<apz> &v) throw()
    {
        assert(v.size()>0);
        size_t    np = 0;        // number of positive
        size_t    nn = 0;        // number of negative
        for(size_t i=v.size();i>0;--i)
        {
            switch(  v[i].s )
            {
                case __zero__:
                    break;

                case positive:
                    ++np;
                    break;

                case negative:
                    ++nn;

                    break;
            }
        }

        assert(nn>0||np>0);
        switch( __sign::of(np,nn) )
        {
            case positive: // do nothing
                assert(np>nn);
                break;

            case negative: // change signs
                assert(np<nn);
                change_signs(v);
                break;

            case __zero__: // change only if first sign is negative
                assert(nn==np);
                if(negative==first_sign(v)) change_signs(v);
                break;
        }
    }



    static inline sign_type  first_sign(const readable<int64_t> &v) throw()
    {
        const size_t n = v.size();
        for(size_t i=1;i<=n;++i)
        {
            switch(__sign::of(v[i]) )
            {
                case __zero__: continue;
                case positive: return positive;
                case negative: return negative;
            }
        }
        return __zero__;
    }

    static inline void       change_signs(writable<int64_t> &v) throw() {
        for(size_t i=v.size();i>0;--i)
        {
            v[i] = -v[i];
        }
    }

    void apk:: univocal(writable<int64_t> &v) throw()
    {
        assert(v.size()>0);
        size_t    np = 0;        // number of positive
        size_t    nn = 0;        // number of negative
        for(size_t i=v.size();i>0;--i)
        {
            switch(  __sign::of(v[i]) )
            {
                case __zero__:
                    break;

                case positive:
                    ++np;
                    break;

                case negative:
                    ++nn;

                    break;
            }
        }

        assert(nn>0||np>0);
        switch( __sign::of(np,nn) )
        {
            case positive: // do nothing
                assert(np>nn);
                break;

            case negative: // change signs
                assert(np<nn);
                change_signs(v);
                break;

            case __zero__: // change only if first sign is negative
                assert(nn==np);
                if(negative==first_sign(v)) change_signs(v);
                break;
        }
    }



    bool apk:: ortho(matrix<apq> &V)
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
                univocal(u_k);
            }
        }
        V.assign(U);


        return true;
    }


}
