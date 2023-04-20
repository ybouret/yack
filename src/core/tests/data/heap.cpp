#include "yack/ordered/run-time-buffer.hpp"
#include "yack/ordered/compiled-buffer.hpp"
#include "yack/ordered/heap.hpp"


#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/allocator/dyadic.hpp"


#include "yack/utest/run.hpp"
#include "../main.hpp"




using namespace yack;

namespace
{
    struct icompare
    {
        inline int operator()(const int lhs, const int rhs) noexcept {
            return lhs-rhs;
        }
    };

    struct scompare
    {
        inline int operator()(const string &lhs, const string &rhs) noexcept
        {
            return string::compare(lhs,rhs);
        }
    };

    template <typename HEAP>
    static inline void perform( HEAP &h, randomized::bits &ran )
    {
        typedef typename HEAP::mutable_type type;
        static const bool versatile = HEAP::buffer_type::versatile;
        std::cerr << "versatile=" << versatile << std::endl;

        h.release();
        const size_t n = versatile ? 10 + ran.leq(10) : h.capacity();
        for(size_t i=n;i>0;--i)
        {
            type tmp = bring::get<type>(ran);
            h.push(tmp);
            std::cerr << "\t" << h << std::endl;
        }
        std::cerr << "size=" << h.size() << " / " << h.capacity() << std::endl;

    }

    

}

YACK_UTEST(data_heap)
{
    randomized::rand_ ran;


    heap< int,icompare,compiled_buffer<int,5> >              fih;
    heap< int,icompare,run_time_buffer<int,memory::pooled> > dih1;
    heap< int,icompare,run_time_buffer<int,memory::dyadic> > dih2(7);

    std::cerr << "fih:  " << fih.size()  << " / " << fih.capacity()  << std::endl;
    std::cerr << "dih1: " << dih1.size() << " / " << dih1.capacity() << std::endl;
    std::cerr << "dih2: " << dih2.size() << " / " << dih2.capacity() << std::endl;
    std::cerr << std::endl;

    fih.release();
    dih1.release();
    dih2.release();

    std::cerr << "fih:  " << fih.size()  << " / " << fih.capacity()  << std::endl;
    std::cerr << "dih1: " << dih1.size() << " / " << dih1.capacity() << std::endl;
    std::cerr << "dih2: " << dih2.size() << " / " << dih2.capacity() << std::endl;
    std::cerr << std::endl;

    dih1.reserve(4);
    dih2.reserve(4);
    std::cerr << "dih1: " << dih1.size() << " / " << dih1.capacity() << std::endl;
    std::cerr << "dih2: " << dih2.size() << " / " << dih2.capacity() << std::endl;
    std::cerr << std::endl;




    fih.push(1);
    dih1.push(2);    YACK_ASSERT(2==dih1.peek());
    dih1.reserve(3); YACK_ASSERT(2==dih1.peek());
    std::cerr << "dih1: " << dih1.size() << " / " << dih1.capacity() << std::endl;



    {
        heap< int,icompare,compiled_buffer<int,7> >              fih_copy1(fih,as_copy);
        heap< int,icompare,run_time_buffer<int,memory::pooled> > fih_copy2(fih,as_copy);
        heap< int,icompare,run_time_buffer<int,memory::dyadic> > fih_copy3(fih,as_copy);
    }

    perform(fih,ran);
    perform(dih1,ran);
    perform(dih2,ran);





}
YACK_UDONE()
