#include "yack/info/compression/alphabet.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace information;

YACK_UTEST(info_alphabet)
{

    YACK_SIZEOF(codec::glyph<0x08>);
    YACK_OUTPUT(codec::glyph<0x08>::cntl_mini);
    YACK_OUTPUT(codec::glyph<0x08>::cntl_maxi);

    codec::glyph<0x08> ua('a');               std::cerr << ua   << std::endl;
    codec::glyph<0x08> unyt(codec::ESC_NYT);  std::cerr << unyt << std::endl;
    codec::glyph<0x08> uend(codec::ESC_END);  std::cerr << uend << std::endl;
    std::cerr << std::endl;


    YACK_SIZEOF(codec::glyph<0x10>);
    YACK_OUTPUT(codec::glyph<0x10>::cntl_mini);
    YACK_OUTPUT(codec::glyph<0x10>::cntl_maxi);

    codec::glyph<0x10> wa('a');              std::cerr << wa   << std::endl;
    codec::glyph<0x10> wnyt(codec::ESC_NYT); std::cerr << wnyt << std::endl;
    codec::glyph<0x10> wend(codec::ESC_END); std::cerr << wend << std::endl;
    std::cerr << std::endl;

    YACK_OUTPUT(codec::alphabet<0x08>::num_codes);
    YACK_OUTPUT(codec::alphabet<0x08>::num_glyphs);
    YACK_OUTPUT(codec::alphabet<0x08>::data_size);
    std::cerr << std::endl;

    YACK_OUTPUT(codec::alphabet<0x10>::num_codes);
    YACK_OUTPUT(codec::alphabet<0x10>::num_glyphs);
    YACK_OUTPUT(codec::alphabet<0x10>::data_size);
    std::cerr << std::endl;



    uint8_t *w1 = NULL;
    uint8_t *w2 = NULL;
    memory::embed emb[] =
    {
        memory::embed(w1,codec::alphabet<0x08>::data_size),
        memory::embed(w2,codec::alphabet<0x10>::data_size)
    };

    size_t wlen = 0;
    void  *wksp = YACK_MEMORY_EMBED(emb,codec::glyph_ops::memmgr(),wlen);
    std::cerr << "@" << wksp << " +" << wlen << std::endl;

    {
        codec::alphabet<0x08> alpha1(w1);
        codec::alphabet<0x10> alpha2(w2);
    }

    codec::glyph_ops::release(wksp,wlen);
    


}
YACK_UDONE()

