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
            if(__zero__ != q.s) {
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
            if(__zero__ != q.s) {
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
    
    void apk:: definite(writable<apz> &z, apn &z2)
    {
        //----------------------------------------------------------------------
        //
        // find first positive value in g, keep track of signs
        //
        //----------------------------------------------------------------------
        const size_t n = z.size();
        z2.ldz();
        switch(n)
        {
            case 0: return;
            case 1:
                switch(z[1].s)
                {
                    case __zero__: break;
                    case positive:
                    case negative: z[1].ldi(1); z2.ldu(1); break;
                }
                return;
            default:
                break;
        }
        

        //----------------------------------------------------------------------
        //
        // compute metrics
        //
        //----------------------------------------------------------------------
        sign_type    f = __zero__; // fist sign
        size_t       m = 0;        // count of minus signs
        size_t       p = 0;        // count of positive signs
        apn          g = 1;        // current gcd
        {
            //------------------------------------------------------------------
            // find first divider and its sign
            //------------------------------------------------------------------
            size_t       j = 1;
            for(;j<=n;++j)
            {
                const apz &y = z[j];
                switch(y.s)
                {
                    case __zero__: continue;
                    case positive: p=1; f = positive; break;
                    case negative: m=1; f = negative; break;
                }
                g = y.n;
                break;
            }
            assert(p+m<=1);

            //------------------------------------------------------------------
            // find first divider, keep track of signs
            //------------------------------------------------------------------
            for(++j;j<=n;++j)
            {
                const apz &y = z[j];
                switch(y.s)
                {
                    case __zero__: continue;
                    case positive: ++p; break;
                    case negative: ++m;  break;
                }
                g = apn::gcd(y.n,g);
            }
        }

        //----------------------------------------------------------------------
        //
        // simplify
        //
        //----------------------------------------------------------------------
        if( (m>p) || ( (m==p) && negative==f) )
        {
            for(size_t i=n;i>0;--i)
            {
                apz &y = z[i];
                switch(y.s)
                {
                    case __zero__: continue;
                    case positive: coerce(y.s) = negative; break;
                    case negative: coerce(y.s) = positive; break;
                }
                z2 += apn::squared( coerce(y.n) /= g );
            }
        }
        else
        {
            for(size_t i=n;i>0;--i)
            {
                apz &y = z[i];
                switch(y.s)
                {
                    case __zero__: continue;
                    case positive:
                    case negative:
                        z2 += apn::squared( coerce(y.n) /= g );
                        continue;
                }
            }
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
                case __zero__:       break;
                case positive: ++np; break;
                case negative: ++nn; break;
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
