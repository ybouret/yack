
#include "yack/gfx/filter/sobel.hpp"

namespace yack
{
    namespace graphic
    {
        template <>
        const char Sobel<3>:: uuid[] = "Sobel3x3";

        template <>
        const int Sobel<3>:: data[3][3] =
        {
            {-1, 0, 1 },
            {-2, 0, 2 },
            {-1, 0, 1 }
        };
    }

    namespace graphic
    {
        template <>
        const char Sobel<5>:: uuid[] = "Sobel5x5";

        template <>
        const int Sobel<5>:: data[5][5] =
        {
            {-5,  -4, 0, 4, 5 },
            {-8, -10, 0, 10, 8 },
            {-10,-20, 0, 20, 10 },
            {-8, -10, 0, 10, 8 },
            {-5,  -4, 0, 4, 5 },
        };
    }

    namespace graphic
    {
        template <>
        const char Sobel<7>:: uuid[] = "Sobel7x7";

        template <>
        const int Sobel<7>:: data[7][7] =
        {
            { -780,   -720,  -468, 0,  468,  720,  780 },
            { -1080, -1170,  -936, 0,  936, 1170, 1080 },
            { -1404, -1872, -2340, 0, 2340, 1872, 1404 },
            { -1560, -2340, -4680, 0, 4680, 2340, 1560 },
            { -1404, -1872, -2340, 0, 2340, 1872, 1404 },
            { -1080, -1170,  -936, 0,  936, 1170, 1080 },
            { -780,   -720,  -468, 0,  468,  720,  780 },
        };
    }
}
