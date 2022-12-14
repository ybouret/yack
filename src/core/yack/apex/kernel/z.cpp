#include "yack/apex/kernel.hpp"

namespace yack
{

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



}
