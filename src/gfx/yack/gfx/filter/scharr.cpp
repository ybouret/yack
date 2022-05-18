

#include "yack/gfx/filter/scharr.hpp"

namespace yack
{
    namespace graphic
    {
        template <>
        const char Scharr<3>:: uuid[] = "Scharr3x3";

        template <>
        const int Scharr<3>:: data[3][3] =
        {
            {-1, 0, 1 },
            {-3, 0, 3 },
            {-1, 0, 1 }
        };
    }

    namespace graphic
    {
        template <>
        const char Scharr<5>:: uuid[] = "Scharr5x5";

        template <>
        const int Scharr<5>:: data[5][5] =
        {
            {-1, -1, 0, 1, 1 },
            {-2, -2, 0, 2, 2 },
            {-3, -6, 0, 6, 3 },
            {-2, -2, 0, 2, 2 },
            {-1, -1, 0, 1, 1 }
        };
    }


}
