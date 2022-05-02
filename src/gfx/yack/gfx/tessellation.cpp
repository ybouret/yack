
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
        dimensions(dims),
        tess2D( coord(w,h), ncpu)
        {
        }


    }

}

