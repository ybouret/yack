#include "yack/sequence/vector.hpp"
#include "yack/sequence/list.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/dyadic.hpp"

#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    template <typename SEQUENCE>
    static inline void show(const SEQUENCE &seq)
    {
        std::cerr << "category : " << seq.category() << std::endl;
        std::cerr << " |_size  : " << seq.size() << std::endl;
        std::cerr << " |_capa  : " << seq.capacity() << std::endl;
        std::cerr << " |_avail : " << seq.available() << std::endl;
    }

    template <typename T>
    static inline void test_seq(randomized::bits &ran)
    {

        list<T>                  l;
        vector<T,memory::pooled> pv;
        vector<T,memory::global> gv;
        vector<T,memory::dyadic> dv;

        for(size_t iter=0;iter<1000;++iter)
        {
            {
                const uint8_t value = ran.to<uint8_t>();
                l.push_back(value);
                pv.push_back(value);
                gv.push_back(value);
                dv.push_back(value);
            }


            {
                const uint8_t value = ran.to<uint8_t>();
                l.push_front(value);
                pv.push_front(value);
                gv.push_front(value);
                dv.push_front(value);
            }
        }

        show(l);
        show(pv);
        show(gv);
        show(dv);

    }



}

YACK_UTEST(data_sequence)
{
    randomized::rand_ ran;


    size_t capa = 0;
    for(size_t i=0;i<10;++i)
    {
        std::cerr << capa << " => ";
        capa = container::next_capacity(capa);
        std::cerr << capa << std::endl;
    }

    test_seq<int>(ran);
    
    cxx_array<int> ir(10);
    

}
YACK_UDONE()


