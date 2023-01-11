#include "yack/math/api.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {
        // mod2
        template <>
        int mod2_of<int>(const int &x)
        {
            return x*x;
        }



        template <>
        int abs_of<int>(const int &x)
        {
            return std::abs(x);
        }

    }


}
