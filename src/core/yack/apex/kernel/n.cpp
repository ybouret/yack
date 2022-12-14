
#include "yack/apex/kernel.hpp"

namespace yack
{

    void apk:: simplify(writable<apn> &z)
    {
        const size_t n = z.size();
        switch(n)
        {
            case 0: return;
            case 1:
                if(z[1]>0) z[1] = 1;
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
            const apn &q = z[j];
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
            const apn &q = z[j];
            if(q!=0) {
                g = apn::gcd(q,g);
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
