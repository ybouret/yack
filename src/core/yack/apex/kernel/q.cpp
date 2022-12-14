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


}

