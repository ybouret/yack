
#include "yack/gfx/dimensions.hpp"


namespace yack
{
    namespace graphic
    {

        dimensions:: ~dimensions() noexcept
        {
            coerce(w) = 0;
            coerce(h) = 0;
        }

        dimensions:: dimensions(const unit_t width, const unit_t height) noexcept :
        w(width),
        h(height)
        {
            assert(w>0);
            assert(h>0);
        }

        dimensions:: dimensions(const dimensions &other) noexcept :
        w(other.w),
        h(other.h)
        {

        }

        bool dimensions:: matches_dimensions(const dimensions &dims) const noexcept
        {
            return w==dims.w && h==dims.h;
        }

        bool dimensions:: contains(const unit_t i, const unit_t j) const noexcept
        {
            return i>=0&&i<w&&j>=0&&j<h;
        }

        bool dimensions:: contains(const coord &p) const noexcept
        {
            return contains(p.x,p.y);
        }

    }

}

