#include "yack/sequence/vector.hpp"
#include "yack/sequence/list.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/apex.hpp"

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

        std::cerr << "Filled: " << std::endl;
        show(l);
        show(pv);
        show(gv);
        show(dv);
        std::cerr << std::endl;

        while(l.size())
        {
            if(ran.choice())
            {
                l.pop_front();
                pv.pop_front();
                dv.pop_front();
                gv.pop_front();

            }
            else
            {
                l.pop_back();
                pv.pop_back();
                dv.pop_back();
                gv.pop_back();

            }
        }

        std::cerr << "Freed: " << std::endl;
        show(l);
        show(pv);
        show(gv);
        show(dv);

        std::cerr << "Adjust:" << std::endl;
        const T z = 0;
        size_t n1 = 100;
        l.adjust(n1,z);   YACK_ASSERT(n1==l.size());
        pv.adjust(n1,z);  YACK_ASSERT(n1==pv.size());
        gv.adjust(n1,z);  YACK_ASSERT(n1==gv.size());
        dv.adjust(n1,z);  YACK_ASSERT(n1==dv.size());
        show(l);
        show(pv);
        show(gv);
        show(dv);

        n1 /= 2;
        l.adjust(n1,z);   YACK_ASSERT(n1==l.size());
        pv.adjust(n1,z);  YACK_ASSERT(n1==pv.size());
        gv.adjust(n1,z);  YACK_ASSERT(n1==gv.size());
        dv.adjust(n1,z);  YACK_ASSERT(n1==dv.size());
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
    test_seq<apq>(ran);

    cxx_array<int> ir(10);
    cxx_array<apq> qr(100);

    {
        vector<int> toto;
        for(size_t i=1+ran.leq(100);i>0;--i)
        {
            toto << int( ran.in(-100,100) );
        }
        std::cerr << toto << std::endl;
        while(toto.size())
        {
            toto.suppress(ran.in(1,toto.ssize()));
        }
    }

}
YACK_UDONE()


