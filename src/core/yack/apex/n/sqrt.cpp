#include "yack/apex/natural.hpp"

namespace yack
{
    namespace apex
    {

        natural natural::sqrt(const natural &u)
        {
            if( u.is<0>() || u.is<1>() ) return u;
            apn x0 = u;
            x0.shr();
            while(true)
            {
                apn x1 = x0 + u/x0;
                x1.shr();
                if(x1>=x0) break;
                x0.xch(x1);
            }
            return x0;
        }

    }
}
