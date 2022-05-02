#include "yack/gfx/metrics.hpp"


namespace yack
{
    namespace graphic
    {
        metrics:: ~metrics() throw()
        {
            coerce(w) = 0;
            coerce(h) = 0;
            coerce(n) = 0;
            coerce(d) = 0;
            coerce(s) = 0;
            coerce(b) = 0;
        }

        metrics:: metrics(const metrics &other) throw() :
        w(other.w),
        h(other.h),
        n(other.n),
        d(other.d),
        s(other.s),
        b(other.b)
        {
        }

        metrics:: metrics(const unit_t W, const unit_t H, const unit_t D) throw() :
        w(W),
        h(H),
        n(w*h),
        d(D),
        s(w*d),
        b(n*d)
        {
            assert(w>0);
            assert(h>0);
            assert(d>0);
        }

    }

}

