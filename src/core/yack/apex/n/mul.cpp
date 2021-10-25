
#include "yack/apex/natural.hpp"

namespace yack
{
    namespace apex
    {
        
        YACK_APN_BINARY_REP(natural operator*,{ YACK_APN_BINARY_IMPL(natural::mul); })
        YACK_APN_UNARY_REP(natural & natural:: operator*=,{YACK_APN_UNARY_IMPL(*); } )

        natural natural:: mul(const handle &l, const handle &r)
        {
            return lmul(l,r);
        }

        natural natural:: squared(const natural &n)
        {
            return _lsqr(n);
        }

    }

}
