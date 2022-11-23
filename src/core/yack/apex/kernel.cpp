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

    void apk:: q2n(writable<apq> &Q)
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
                q = z;
            } return;

                // make all denominators as 1
            default: {
                const apq _ = lcm(Q,size_t(1),n);
                for(size_t i=n;i>0;--i) { Q[i] *= _; assert(1==Q[i].den); }
            } break;
        }
        assert(n>=2);

        // find first positive value
        apn    g=1;
        size_t j=1;
        for(;j<=n;++j)
        {
            const apq &q = Q[j];
            if(q!=0) {
                g = q.num.n;
                break;
            }
        }

        // compute with other positive values
        for(++j;j<=n;++j)
        {
            const apq &q = Q[j];
            if(q!=0) {
                g = apn::gcd(q.num.n,g);
            }
        }

        // simplify
        for(size_t i=n;i>0;--i)
        {
            Q[i] /= g;
            assert(1==Q[i].den);
        }



    }

}
