#include "yack/gfx/filter/prewitt.hpp"

namespace yack
{
    namespace graphic
    {
        template <>
        const char Prewitt<3>:: uuid[] = "Prewitt3x3";

        template <>
        const int Prewitt<3>:: data[3][3] =
        {
            {-1, 0, 1 },
            {-1, 0, 1 },
            {-1, 0, 1 }
        };
    }

    namespace graphic
    {
        template <>
        const char Prewitt<5>:: uuid[] = "Prewitt5x5";

        template <>
        const int Prewitt<5>:: data[5][5] =
        {
            {-2, -1, 0, 1, 2 },
            {-2, -1, 0, 1, 2 },
            {-2, -1, 0, 1, 2 },
            {-2, -1, 0, 1, 2 },
            {-2, -1, 0, 1, 2 }
        };
    }

    namespace graphic
    {
        template <>
        const char Prewitt<7>:: uuid[] = "Prewitt7x7";

        template <>
        const int Prewitt<7>:: data[7][7] =
        {
            {-3, -2, -1, 0, 1, 2, 3},
            {-3, -2, -1, 0, 1, 2, 3},
            {-3, -2, -1, 0, 1, 2, 3},
            {-3, -2, -1, 0, 1, 2, 3},
            {-3, -2, -1, 0, 1, 2, 3},
            {-3, -2, -1, 0, 1, 2, 3},
            {-3, -2, -1, 0, 1, 2, 3}
        };
    }
}
