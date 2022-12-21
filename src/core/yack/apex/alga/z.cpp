
#include "yack/apex/alga.hpp"

namespace yack
{
    void alga:: simplify(writable<apz> &z)
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
        {
            size_t   j = 1;
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
        }
        //----------------------------------------------------------------------
        // simplify
        //----------------------------------------------------------------------
        for(size_t i=n;i>0;--i)
        {
            z[i] /= g;
        }
    }

    void alga:: definite(writable<apz> &z, apn &z2)
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
        // simplify/change sign
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

}


