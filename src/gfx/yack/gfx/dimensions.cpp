
#include "yack/gfx/dimensions.hpp"


namespace yack
{
    namespace graphic
    {

        dimensions:: ~dimensions() throw()
        {
            coerce(w) = 0;
            coerce(h) = 0;
        }

        dimensions:: dimensions(const unit_t width, const unit_t height) throw() :
        w(width),
        h(height)
        {
            assert(w>0);
            assert(h>0);
        }

        dimensions:: dimensions(const dimensions &other) throw() :
        w(other.w),
        h(other.h)
        {

        }

        bool dimensions:: matches_dimensions(const dimensions &dims) const throw()
        {
            return w==dims.w && h==dims.h;
        }

        bool dimensions:: contains(const unit_t i, const unit_t j) const throw()
        {
            return i>=0&&i<w&&j>=0&&j<h;
        }

        bool dimensions:: contains(const coord &p) const throw()
        {
            return contains(p.x,p.y);
        }

    }

}

