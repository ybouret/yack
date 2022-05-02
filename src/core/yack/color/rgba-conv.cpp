#include "yack/color/rgba-conv.hpp"

namespace yack
{
    namespace color
    {

        rgba_conv:: ~rgba_conv() throw()
        {
            coerce(d) = 0;
        }

        rgba_conv:: rgba_conv(const unit_t depth) throw() :
        d(depth)
        {
            assert(depth>0);
        }
    }

}

