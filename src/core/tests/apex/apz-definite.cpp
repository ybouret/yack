

#include "yack/apex/kernel.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

namespace {
    static inline void do_def(writable<apz> &z)
    {
        apn z2 = 0xffffffff;
        std::cerr << z << " -> ";
        apk::definite(z,z2);
        std::cerr << z << " | " << z2 << std::endl;
    }

    static inline void fillz(writable<apz> &z, randomized::bits &ran)
    {
        for(size_t i=z.size();i>0;--i)
        {
            z[i] = ran.in(-10,10);
        }
    }

}

YACK_UTEST(apz_definite)
{

    randomized::rand_ ran;

    vector<apz> z;
    do_def(z);
    for(size_t cycle=1;cycle<=4;++cycle)
    {
        std::cerr << std::endl;
        z << 0;
        z.ld(0);
        do_def(z);
        for(size_t iter=0;iter<8;++iter)
        {
            fillz(z,ran);
            do_def(z);
        }
    }


}
YACK_UDONE()
