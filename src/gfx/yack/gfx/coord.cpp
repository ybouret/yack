
#include "yack/gfx/coord.hpp"


namespace yack
{
    namespace graphic
    {

        const coord coords::delta[8] =
        {

            // axis
            coord(1,0),
            coord(0,1),
            coord(-1,0),
            coord(0,-1),

            // bisect
            coord(1,1),
            coord(-1,1),
            coord(-1,-1),
            coord(1,-1)

        };

    }

}
