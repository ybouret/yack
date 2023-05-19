
#include "yack/apex/flak.hpp"
#include "yack/system/imported.hpp"

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


    }
}
