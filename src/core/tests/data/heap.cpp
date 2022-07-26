#include "yack/ordered/heap.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/utest/run.hpp"
#include "yack/comparison.hpp"
#include "../main.hpp"

using namespace yack;

namespace
{
    template <typename T>
    struct  hincr
    {
        inline int operator()(const T &lhs, const T &rhs) throw()
        {
            return comparison::increasing<T>(lhs,rhs);
        }
    };

    template <typename T>
    struct  hdecr
    {
        inline int operator()(const T &lhs, const T &rhs) throw()
        {
            return comparison::decreasing<T>(lhs,rhs);
        }
    };

    template <typename T,  typename ALLOCATOR>
    static inline void testHeap( randomized::bits &ran )
    {
        typedef heap<T,hincr<T>,ALLOCATOR> IncrHeap;

        {IncrHeap Hi0;}

        IncrHeap Hi(1,as_capacity);

        for(size_t i=20+ran.leq(20);i>0;--i)
        {
            const T tmp = bring::get<T>(ran);
            Hi.push(tmp);
            std::cerr << "heap: " << Hi.size() << " / " << Hi.capacity() << " / bytes=" << Hi.granted() << std::endl;
        }

        std::cerr << std::endl;
    }

}

YACK_UTEST(data_heap)
{
    randomized::rand_ ran;

    testHeap<int,memory::pooled>(ran);
    testHeap<double,memory::dyadic>(ran);
    testHeap<string,memory::dyadic>(ran);

}
YACK_UDONE()

