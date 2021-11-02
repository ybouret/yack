#include "yack/info/modulation/echo.hpp"
#include "yack/info/modulation/mtf.hpp"
#include "yack/info/modulation/delta.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{

    static void test_mod_codec(information::modulation &enc,
                               information::modulation &dec,
                               randomized::bits        &ran)
    {

        for(size_t i=0;i<256;++i)
        {
            enc.reset();
            dec.reset();
            const uint8_t x = uint8_t(i);   //std::cerr << "x=" << int(x) << std::endl;
            const uint8_t y = enc.alter(x); //std::cerr << "y=" << int(y) << std::endl;
            const uint8_t z = dec.alter(y); //std::cerr << "z=" << int(z) << std::endl;
            YACK_ASSERT(z==x);
        }

        enc.reset();
        dec.reset();
        for(size_t i=1024*16;i>0;--i)
        {
            const uint8_t x = ran.full<uint8_t>();//std::cerr << "x=" << int(x) << std::endl;
            const uint8_t y = enc.alter(x);       //std::cerr << "y=" << int(y) << std::endl;
            const uint8_t z = dec.alter(y);       //std::cerr << "z=" << int(z) << std::endl;
            YACK_ASSERT(z==x);
        }


    }

}


YACK_UTEST(info_mod)
{
    randomized::rand_ ran;

    information::echo echo_enc;
    information::echo echo_dec;
    std::cerr << "Testing echo" << std::endl;
    test_mod_codec(echo_enc,echo_dec,ran);

    information::mtf_encoder mtf_enc;
    information::mtf_decoder mtf_dec;
    std::cerr << "Testing mtf" << std::endl;
    test_mod_codec(mtf_enc,mtf_dec,ran);


    information::delta_encoder del_enc;
    information::delta_decoder del_dec;
    std::cerr << "Testing delta" << std::endl;
    test_mod_codec(del_enc,del_dec,ran);

}
YACK_UDONE()

