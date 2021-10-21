#include "yack/hashing/des64.hpp"
#include "yack/hashing/ibj64.hpp"
#include "yack/type/hexa.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace hashing;

namespace
{
    static inline void test_h64( hash64 &H )
    {
        std::cerr << "[Testing " << H.protocol() << "]" << std::endl;
        uint32_t l=0, r=0;
        H(l,r);
        std::cerr << "0 => " << l << "," << r << std::endl;
        uint8_t k=0;
        for(size_t i=0;i<16;++i)
        {
            for(size_t j=0;j<16;++j,++k)
            {
                std::cerr << ' ' << hexa::uppercase_text[H.mix(k)];
            }
            std::cerr << std::endl;
        }
    }
    
}

YACK_UTEST(hash64)
{
    des64 des;
    const bool des64_is_ok = des.test();
    YACK_CHECK(des64_is_ok);

    ibj64 ibj;

    test_h64(des);
    test_h64(ibj);
    

}
YACK_UDONE()

