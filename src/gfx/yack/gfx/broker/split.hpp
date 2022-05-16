
//! \file

#ifndef YACK_GFX_BROKER_SPLIT_INCLUDED
#define YACK_GFX_BROKER_SPLIT_INCLUDED 1

#include "yack/gfx/pixmaps.hpp"

namespace yack
{
    namespace graphic
    {
        
        struct broker_split
        {
            template <typename PIXEL,
            typename T>
            void build(pixmaps<T>          &target,
                       const pixmap<PIXEL> &source,
                       broker              &device)
            {
                
            }
        };
        
    }
    
}

#endif

