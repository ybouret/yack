#include "yack/info/compression/alphabet.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace information;

YACK_UTEST(info_alphabet)
{
    YACK_SIZEOF(codec::glyph<uint8_t>);
    YACK_SIZEOF(codec::glyph<uint16_t>);


}
YACK_UDONE()

