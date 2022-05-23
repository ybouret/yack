#include "yack/gfx/pixel.hpp"
#include "yack/gfx/rgb.hpp"
#include <cmath>

namespace yack
{
    namespace graphic
    {
        template <> uint8_t pixel<uint8_t>:: zero()   { return 0; }
        template <> float   pixel<float>  :: zero()   { return 0; }
        template <> double  pixel<double> :: zero()   { return 0; }

        template <> rgb   pixel<rgb>  :: zero()   { return rgb(0,0,0);       }
        template <> rgba  pixel<rgba> :: zero()   { return rgba(0,0,0,0xff); }


        template <> bool pixel<uint8_t>:: is_zero(const uint8_t &u) throw() { return u<=0; }
        template <> bool pixel<float>::   is_zero(const float   &f) throw() { return fabs(f) <=0; };
        template <> bool pixel<double>::  is_zero(const double  &d) throw() { return fabs(d) <=0; };

        template <> bool pixel<rgb>::  is_zero(const rgb  &c) throw() { return unsigned(c.r)+unsigned(c.g)+unsigned(c.b) <= 0; }
        template <> bool pixel<rgba>:: is_zero(const rgba &c) throw() { return unsigned(c.r)+unsigned(c.g)+unsigned(c.b) <= 0; }


    }
}

