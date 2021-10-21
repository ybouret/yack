#include "yack/hashing/hash64-des.hpp"
#include "yack/hashing/hash64-ibj.hpp"

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

