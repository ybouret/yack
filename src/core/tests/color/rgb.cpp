#include "yack/color/rgba.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(color_rgb)
{
    YACK_SIZEOF(color::rgb);
    YACK_SIZEOF(color::rgba);
}
YACK_UDONE()

