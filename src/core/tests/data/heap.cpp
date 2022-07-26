#include "yack/ordered/heap.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/utest/run.hpp"
#include "yack/comparison.hpp"

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
    static inline void testHeap()
    {
        typedef heap<T,hincr<T>,ALLOCATOR> IncrHeap;

        {IncrHeap Hi0;}

        IncrHeap Hi(1,as_capacity);
        Hi.free();
    }

}

YACK_UTEST(data_heap)
{

    testHeap<int,memory::pooled>();
    testHeap<int,memory::dyadic>();

}
YACK_UDONE()

