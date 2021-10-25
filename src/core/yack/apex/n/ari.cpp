

#include "yack/apex/natural.hpp"


namespace yack
{
    namespace apex
    {
        natural  natural:: mod_inv( const natural &b, const natural &n )
        {
            natural n0 = n;
            natural b0 = b;
            natural t0 = 0;
            natural t  = 1;
            natural q  = n0/b0;
            natural r  = n0 - q*b0;

            while( r>0 )
            {
                const natural lhs = t0;
                const natural rhs = q*t;

                t0 = t;

                if( compare(lhs,rhs) >= 0 )
                {
                    assert(lhs>=rhs);
                    t = ( (lhs-rhs) % n );
                    assert(t<n);
                }
                else
                {
                    assert(rhs>lhs);
                    t  = n - ( (rhs-lhs)%n );
                    assert(t<=n);
                }

                n0 = b0;
                b0 = r;
                q  = n0/b0;
                r  = n0 - q * b0;
            }

            // normally an error if b0 != 1
            return t;
        }
    }

}

