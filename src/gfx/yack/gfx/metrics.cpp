#include "yack/gfx/metrics.hpp"


namespace yack
{
    namespace graphic
    {
        metrics:: ~metrics() throw()
        {
            coerce(n) = 0;
            coerce(d) = 0;
            coerce(s) = 0;
            coerce(b) = 0;
        }

        metrics:: metrics(const metrics &other) throw() :
        dimensions(other),
        n(other.n),
        d(other.d),
        s(other.s),
        b(other.b)
        {
        }

        metrics:: metrics(const unit_t W, const unit_t H, const unit_t D) throw() :
        dimensions(W,H),
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

