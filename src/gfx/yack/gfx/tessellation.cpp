
#include "yack/gfx/tessellation.hpp"

namespace yack
{
    namespace graphic
    {

        tessellation:: ~tessellation() throw()
        {
        }

        tessellation:: tessellation(const metrics &info,
                                    const size_t   ncpu) :
        tess2D( coord(info.w,info.h), ncpu)
        {
        }


    }

}

