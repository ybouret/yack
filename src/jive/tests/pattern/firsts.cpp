#include "yack/jive/pattern/first-bytes.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(firsts)
{
    randomized::rand_ ran;
    YACK_SIZEOF(jive::domain);



    jive::first_bytes p;
    jive::domains     doms;

    for(size_t iter=0;iter<4;++iter)
    {
        p.release();
        doms.release();
        size_t lower = 0;
        while(lower<255) {
            size_t upper = lower + ran.leq(16);
            if(upper>255) upper=255;
            doms.push_back( new jive::domain(lower,upper) );
            lower = upper;
            //std::cerr << *doms.tail << std::endl;
        }
        YACK_ASSERT(0x00==doms.head->lower);
        YACK_ASSERT(0xff==doms.tail->upper);
        randomized::shuffle::list(doms,ran);
        while(doms.size)
        {
            p << doms.pop_back();
            std::cerr << p << std::endl;
        }
        YACK_ASSERT(1==p.size);
        YACK_ASSERT(0x00==p.head->lower);
        YACK_ASSERT(0xff==p.head->upper);
    }

    {
        p.all();
        std::cerr << p << std::endl;
        uint8_t arr[256];
        for(size_t i=0;i<256;++i) arr[i] = uint8_t(i);
        randomized::shuffle::data(arr,256,ran);
        for(size_t i=0;i<256;++i)
        {
            p -= arr[i];
            std::cerr << p << std::endl;
        }
    }

}
YACK_UDONE()

