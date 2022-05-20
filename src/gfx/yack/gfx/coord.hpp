//! \file

#ifndef YACK_GFX_COORD_INCLUDED
#define YACK_GFX_COORD_INCLUDED 1

#include "yack/type/v2d.hpp"


namespace yack
{
    namespace graphic
    {
        typedef v2d<unit_t> coord;  //!< alias

        //! some coordinates
        struct coords
        {
            static const coord delta[8]; //!< 4+4 delta around origin
        };
    }

}

#endif
