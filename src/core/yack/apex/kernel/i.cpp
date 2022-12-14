
#include "yack/apex/kernel.hpp"
#include "yack/arith/gcd.h"

namespace yack
{

    static inline uint64_t abs64_of(const int64_t z) throw()
    {
        if(z<0)
        {
            return static_cast<uint64_t>(-z);
        }
        else
        {
            return static_cast<uint64_t>(z);
        }
    }

    void apk:: simplify(writable<int64_t> &z) throw()
    {
        const size_t n = z.size();
        switch(n)
        {
            case 0: return;
            case 1:
                switch( __sign::of(z[1]) )
                {
                    case __zero__:           break;
                    case positive: z[1]=  1; break;
                    case negative: z[1]= -1; break;
                }
                return;
            default:
                break;
        }

        //----------------------------------------------------------------------
        // find first positive value
        //----------------------------------------------------------------------
        uint64_t     g = 1;
        size_t       j = 1;
        for(;j<=n;++j)
        {
            const uint64_t q = abs64_of(z[j]);
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
            const uint64_t q = abs64_of(z[j]);
            if(q!=0) {
                g = yack_gcd64_(q,g);
            }
        }

        const int64_t gg = static_cast<int64_t>(g);
        //----------------------------------------------------------------------
        // simplify
        //----------------------------------------------------------------------
        for(size_t i=n;i>0;--i)
        {
            z[i] /= gg;
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

}
