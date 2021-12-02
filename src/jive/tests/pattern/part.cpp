#include "yack/jive/pattern/partition.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(part)
{
    randomized::rand_ ran;
    YACK_SIZEOF(jive::domain);



    jive::partition p;
    p.add('h','r'); std::cerr << p << std::endl;
    p.add('a');     std::cerr << p << std::endl;
    p.add('b');     std::cerr << p << std::endl;
    p.add('d');     std::cerr << p << std::endl;
    p.add('e');     std::cerr << p << std::endl;
    p.add('c');     std::cerr << p << std::endl;

    p.add('Z','f'); std::cerr << p << std::endl;
    p.add('g');     std::cerr << p << std::endl;

    cxx_list_of<jive::domain> doms;

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
            p.grow(doms.pop_back());
            std::cerr << p << std::endl;
        }
        YACK_ASSERT(1==p.size);
        YACK_ASSERT(0x00==p.head->lower);
        YACK_ASSERT(0xff==p.head->upper);
    }

}
YACK_UDONE()

