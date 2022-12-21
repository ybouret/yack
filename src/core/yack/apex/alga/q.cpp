#include "yack/apex/alga.hpp"

namespace yack
{
    void alga::   simplify( writable<apq> &v )
    {
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
        const apn factor = lcm(v,size_t(1),n);
        apn       g      = 1;
        size_t    i      = 1;
        for(;i<=n;++i)
        {
            apq &q = v[i];
            if(q.num.s!=__zero__)
            {
                q *= factor;   assert(1==q.den);
                g  = q.num.n;
                break;
            }
        }

        for(++i;i<=n;++i)
        {
            apq &q = v[i];
            if(q.num.s!=__zero__)
            {
                q *= factor;   assert(1==q.den);
                g  = apn::gcd(q.num.n,g);
            }
        }

        for(size_t i=n;i>0;--i) {
            v[i] /= g;
        }

    }


}


