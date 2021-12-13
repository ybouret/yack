#include "yack/randomized/rand.hpp"
#include "yack/randomized/park-miller.hpp"
#include "yack/randomized/shuffle.hpp"
#include "yack/randomized/in2d.hpp"

#include "yack/data/list/raw.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/seed.hpp"



#include <cstring>

using namespace yack;

namespace
{

    template <typename T>
    static inline void draw( randomized::bits &ran )
    {
        std::cerr << "uint" << sizeof(T)*8 << "_t:";
        for(size_t i=0;i<4;++i)
        {
            std::cerr << " " << uint64_t( ran.to<T>() );
        }
        std::cerr << std::endl;
    }

    struct node_t
    {
        node_t *next;
        node_t *prev;
        size_t  indx;
    };

    static void test_bits( randomized::bits &ran )
    {
        draw<uint8_t>(ran);
        draw<uint16_t>(ran);
        draw<uint32_t>(ran);
        draw<uint64_t>(ran);

        size_t       data[20];
        const size_t nd = sizeof(data)/sizeof(data[0]);
        for(unsigned i=0;i<nd;++i)
        {
            data[i] = i;
        }

        randomized::shuffle::data(data,nd,ran);


        node_t nodes[20];
        memset(nodes,0,sizeof(nodes));
        const size_t nn = sizeof(nodes)/sizeof(nodes[0]);
        raw_list_of<node_t> l;
        for(size_t i=0;i<nn;++i)
        {
            l.push_back(nodes+i)->indx=i;
        }
        randomized::shuffle::list(l,ran);
        
        l.restart();
        
        double sum = 0;
        size_t num = 100000;
        for(size_t i=num;i>0;--i)
        {
            sum += ran();
        }
        sum /= num;
        std::cerr << "ave=" << sum << std::endl;
        
        
    }

}

#include "yack/type/ints.hpp"
#include "yack/ios/fmt/hexa.hpp"


template <typename T>
static inline uint32_t find_conv(volatile uint32_t curr)
{
    static const uint32_t umax = 0xffffffff;
    while(true)
    {
        volatile T        f    = static_cast<T>(curr);
        volatile uint32_t back = static_cast<uint32_t>(f);
        if(back!=curr) return curr;
        if(umax==curr) return 0;
        ++curr;
    }
}


YACK_UTEST(rand_bits)
{
    randomized::rand_       ran;
    randomized::ParkMiller  ranPM( system_seed::get<randomized::ParkMiller::word_type>() );

    std::cerr << "Testing rand()" << std::endl;
    test_bits(ran);

    std::cerr << "Testing Park-Miller" << std::endl;
    test_bits(ranPM);

    const uint32_t uf = find_conv<float>(1);
    std::cerr << "uf = " << uf << std::endl;
#if 0
    const uint32_t ud = find_conv<double>(1);
    std::cerr << "ud = " << ud << std::endl;

    const uint32_t ul = find_conv<long double>(1);
    std::cerr << "ul = " << ul << std::endl;
#endif


}
YACK_UDONE()


