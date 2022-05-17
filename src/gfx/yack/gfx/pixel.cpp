#include "yack/gfx/pixel.hpp"
#include "yack/gfx/rgb.hpp"


namespace yack
{
    namespace graphic
    {
        template <> uint8_t pixel<uint8_t>:: zero()   { return 0; }
        template <> float   pixel<float>  :: zero()   { return 0; }
        template <> double  pixel<double> :: zero()   { return 0; }

        template <> rgb   pixel<rgb>  :: zero()   { return rgb(0,0,0);       }
        template <> rgba  pixel<rgba> :: zero()   { return rgba(0,0,0,0xff); }

    }
}
