#include "yack/apex/alga.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    void alga::   simplify( writable<apq> &v )
    {
        // process cases
        const size_t n = v.size();
        switch(n)
        {
            case 0: return;
            case 1: {
                apq &q = v[1];
                switch(q.num.s)
                {
                    case __zero__: return;
                    case positive: q= 1; return;
                    case negative: q=-1; return;
                }
            } return;

            default:
                break;
        }

        assert(n>=2);

        apn       g  = 1;                      // divider
        {
            const apn m  = lcm(v,size_t(1),n); // mulitplier
            size_t    i      = 1;
            for(;i<=n;++i)
            {
                apq &q = v[i];
                if(q.num.s!=__zero__)
                {
                    q *= m;       assert(1==q.den);
                    g  = q.num.n;
                    break;
                }
            }

            // propagate
            for(++i;i<=n;++i)
            {
                apq &q = v[i];
                if(q.num.s!=__zero__)
                {
                    q *= m;   assert(1==q.den);
                    g  = apn::gcd(q.num.n,g);
                }
            }
        }

        // finalize
        for(size_t i=n;i>0;--i) {
            const apz &z  = v[i].num;
            if(z.s!=__zero__) coerce(z.n) /= g;
        }

    }

    void alga::   univocal( writable<apq> &v )
    {
        // process cases
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
                    case negative: q= 1; return;
                }
            } return;

            default:
                break;
        }

        assert(n>=2);

        sign_type fs = __zero__;  // first sign
        size_t    np = 0;         // number of positive
        size_t    nn = 0;         // number of negative
        apn       g  = 1;         // divider
        {
            const apn m  = lcm(v,size_t(1),n); // mulitplier
            size_t    i      = 1;
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

            // propagate
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

        if(nn>np || (nn==np&&negative==fs))
        {
            // changing signs
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
            // keeping signs
            for(size_t i=n;i>0;--i)
                coerce(v[i].num.n) /= g;

        }
    }

    bool alga:: colinear(const readable<apq> &lhs, const readable<apq> &rhs)
    {
        assert( lhs.size() == rhs.size() );
        auto_ptr<const apq> f = NULL;

        for(size_t i=lhs.size();i>0;--i)
        {
            const apq &l = lhs[i];
            const apq &r = rhs[i];
            switch( __sign::pair(l.num.s,r.num.s) )
            {
                case zz_pair: continue; // compatible;
                case zp_pair:
                case pz_pair:
                case zn_pair:
                case nz_pair:
                    return false; // incompatible

                case nn_pair:
                case pp_pair: {
                    const apq rho(l.num.n,r.num.n);
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
                    const apq rho(l.num.n,r.num.n); coerce(rho.num.s) = negative;
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


