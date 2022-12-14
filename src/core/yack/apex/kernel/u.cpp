#include "yack/apex/kernel.hpp"
#include "yack/arith/gcd.h"

namespace yack
{

    void apk:: simplify(writable<uint64_t> &u) throw()
    {
        const size_t n = u.size();
        switch(n)
        {
            case 0: return;
            case 1:
                if(u[1]>0) u[1] = 1;
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
            const uint64_t q = u[j];
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
            const uint64_t q = u[j];
            if(q!=0) {
                g = yack_gcd64_(q,g);
            }
        }

        //----------------------------------------------------------------------
        // simplify
        //----------------------------------------------------------------------
        for(size_t i=n;i>0;--i)
        {
            u[i] /= g;
        }
    }

}
