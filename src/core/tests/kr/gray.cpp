#include "yack/kr/bc/gray.hpp"
#include "yack/utest/run.hpp"

using namespace yack;
using namespace crypto;

template <typename T>
static inline void test_gray_code(randomized::bits &ran)
{
    
    for(size_t i=0; i < 100000; ++i)
    {
        const T org = ran.full<T>();
        const T enc = gray<T>::encode(org);
        const T dec = gray<T>::decode(enc);
        if(i<10)
        {
            std::cerr << uint64_t(org) << " =>" << uint64_t(enc) << " => " << uint64_t(dec) << std::endl;
        }
        YACK_ASSERT(dec==org);
    }
    
}

YACK_UTEST(kr_gray)
{
    randomized::rand_ ran;
    
    test_gray_code<uint8_t>(ran);
    test_gray_code<uint16_t>(ran);
    test_gray_code<uint32_t>(ran);
    test_gray_code<uint64_t>(ran);

}
YACK_UDONE()

