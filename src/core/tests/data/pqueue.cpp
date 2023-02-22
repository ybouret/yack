
#include "yack/ordered/pqueue.hpp"
#include "yack/utest/run.hpp"
#include "yack/comparison.hpp"
#include "../main.hpp"
#include "yack/memory/allocator/dyadic.hpp"

using namespace yack;

using namespace yack;

namespace
{
    template <typename T>
    struct  hincr
    {
        inline int operator()(const T &lhs, const T &rhs) noexcept
        {
            return comparison::increasing<T>(lhs,rhs);
        }
    };

    template <typename T>
    struct  hdecr
    {
        inline int operator()(const T &lhs, const T &rhs) noexcept
        {
            return comparison::decreasing<T>(lhs,rhs);
        }
    };

    template <typename T,  typename ALLOCATOR>
    static inline void testHeap( randomized::bits &ran )
    {
        typedef pqueue<T,hincr<T>,ALLOCATOR> IncrHeap;
        typedef pqueue<T,hdecr<T>,ALLOCATOR> DecrHeap;

        {IncrHeap Hi0;}

        IncrHeap Hi(1,as_capacity);
        DecrHeap Hd;

        for(size_t i=20+ran.leq(20);i>0;--i)
        {
            const T tmp = bring::get<T>(ran);
            Hi.push(tmp);
            Hd.push(tmp);
            std::cerr << "Hi: " << std::setw(3) << Hi.size() << " / " << std::setw(3) << Hi.capacity() << " peek=" << Hi.peek() << std::endl;
            std::cerr << "Hd: " << std::setw(3) << Hd.size() << " / " << std::setw(3) << Hd.capacity() << " peek=" << Hd.peek() << std::endl;
        }

        YACK_ASSERT(Hi.size()==Hd.size());

        while( Hi.size() )
        {
            std::cerr << "Hi: " << Hi.peek() << " | Hd: " << Hd.peek() << std::endl;
            Hi.pop();
            Hd.pop();
        }

        std::cerr << std::endl;
    }

}

YACK_UTEST(data_pqueue)
{
    randomized::rand_ ran;

    testHeap<int,memory::pooled>(ran);
    testHeap<double,memory::dyadic>(ran);
    testHeap<string,memory::dyadic>(ran);

    YACK_SIZEOF(int);
    YACK_SIZEOF(double);
    YACK_SIZEOF(string);
    
}
YACK_UDONE()
