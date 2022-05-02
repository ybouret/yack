
#include "yack/gfx/tessellation.hpp"

namespace yack
{
    namespace graphic
    {

        tessellation:: ~tessellation() throw()
        {
        }

        tessellation:: tessellation(const dimensions &dims,
                                    const size_t     ncpu) :
        tess2D( coord(dims.w,dims.h), ncpu)
        {
        }


    }

}

