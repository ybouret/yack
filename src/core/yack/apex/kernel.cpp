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

}
