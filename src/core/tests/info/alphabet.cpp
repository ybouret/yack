#include "yack/info/compression/alphabet.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace information;

YACK_UTEST(info_alphabet)
{

    YACK_SIZEOF(codec::glyph<0x08>);
    YACK_OUTPUT(codec::glyph<0x08>::cntl_mini);
    YACK_OUTPUT(codec::glyph<0x08>::cntl_maxi);

    codec::glyph<0x08> ua   = 'a';             std::cerr << ua   << std::endl;
    codec::glyph<0x08> unyt = codec::ESC_NYT;  std::cerr << unyt << std::endl;
    codec::glyph<0x08> uend = codec::ESC_END;  std::cerr << uend << std::endl;


    YACK_SIZEOF(codec::glyph<0x10>);
    YACK_OUTPUT(codec::glyph<0x10>::cntl_mini);
    YACK_OUTPUT(codec::glyph<0x10>::cntl_maxi);

    codec::glyph<0x10> wa   = 'a';            std::cerr << wa   << std::endl;
    codec::glyph<0x10> wnyt = codec::ESC_NYT; std::cerr << wnyt << std::endl;
    codec::glyph<0x10> wend = codec::ESC_END; std::cerr << wend << std::endl;


}
YACK_UDONE()

