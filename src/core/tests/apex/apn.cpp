
#include "yack/apex/natural.hpp"
#include "yack/arith/base2.hpp"
#include "yack/utest/run.hpp"


using namespace yack;

namespace
{
    
}

YACK_UTEST(apex_apn)
{

    std::cerr << "apn.min_words_bytes=" << apn::min_words_bytes << std::endl;
    std::cerr << "apn.min_words_size =" << apn::min_words_size  << std::endl;
    std::cerr << "apn.min_words_exp2 =" << apn::min_words_exp2  << std::endl;

    uprng ran;
    for(size_t bits=0;bits<=64;++bits)
    {
        for(size_t iter=0;iter<16;++iter)
        {
            const apex::unsigned_type u = ran.gen<apex::unsigned_type>(bits);
            YACK_ASSERT(bits==bits_for(u));
            apn n = u;
            std::cerr << "u=" << u << std::endl;
            YACK_ASSERT(n.lsu()==u);
            YACK_ASSERT(bytes_for(u)==n.size());
            YACK_ASSERT(n.bits()==bits);
        }
    }

    YACK_SIZEOF(apn);

}
YACK_UDONE()
