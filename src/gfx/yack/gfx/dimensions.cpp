
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

    }

}

