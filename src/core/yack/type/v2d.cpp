
#include "yack/type/v2d.hpp"
#include "yack/type/constants.hpp"
#include <cmath>

namespace yack
{

    template<> float v2d<float>:: angle() const throw()
    {
        if( fabsf(y) > 0 )
        {
            return atan2f(y,x);
        }
        else
        {
            return (x<0) ? YACK_PI_F : 0;
        }
    }


    template<> double v2d<double>:: angle() const throw()
    {
        if( fabs(y) > 0 )
        {
            return atan2(y,x);
        }
        else
        {
            return (x<0) ? YACK_PI_D : 0;
        }
    }

    template<> long double v2d<long double>:: angle() const throw()
    {
        if( fabsl(y) > 0 )
        {
            return atan2l(y,x);
        }
        else
        {
            return (x<0) ? YACK_PI_L : 0;
        }
    }

}


