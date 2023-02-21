#include "yack/gfx/metrics.hpp"


namespace yack
{
    namespace graphic
    {
        metrics:: ~metrics() noexcept
        {
            coerce(n) = 0;
            coerce(d) = 0;
            coerce(s) = 0;
            coerce(b) = 0;
        }

        metrics:: metrics(const metrics &other) noexcept :
        dimensions(other),
        n(other.n),
        d(other.d),
        s(other.s),
        b(other.b)
        {
        }

        metrics:: metrics(const unit_t W, const unit_t H, const unit_t D) noexcept :
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

        metrics:: metrics(const dimensions &dims, const unit_t depth) noexcept :
        dimensions(dims),
        n(w*h),
        d(depth),
        s(w*d),
        b(n*d)
        {
            assert(w>0);
            assert(h>0);
            assert(d>0);
        }


    }

}

