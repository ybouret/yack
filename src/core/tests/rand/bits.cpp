#include "yack/randomized/rand.hpp"
#include "yack/randomized/park-miller.hpp"
#include "yack/randomized/shuffle.hpp"
#include "yack/data/raw-list.hpp"
#include "yack/utest/run.hpp"

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
        YACK_OUTPUT(ran.count);
        YACK_OUTPUT(ran.bytes);
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

YACK_UTEST(rand_bits)
{
    randomized::rand_       rans( time(NULL) );
    randomized::ParkMiller  ranPM( time(NULL) );
    
    test_bits(rans);
    test_bits(ranPM);


}
YACK_UDONE()


