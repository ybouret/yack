#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/comparison.hpp"

#include "../main.hpp"
#include <typeinfo>

using namespace yack;

namespace
{

    template <typename T>
    size_t partition( T arr[], const size_t lo, const size_t hi)
    {
        
        const T pivot = arr[ (lo+hi)>>1 ];
        size_t  i     = lo-1;
        size_t  j     = hi+1;
        while(true)
        {
            do ++i; while ( arr[i]<pivot );
            do --j; while ( arr[j]>pivot );
            if(i>=j) return j;
            cswap(arr[i],arr[j]);
        }
    }

    template <typename T>
    void quickSortAlgo( T arr[], const size_t lo, const size_t hi)
    {
        assert(lo>0);
        if(lo<hi)
        {
            const size_t p = partition(arr,lo,hi);
            quickSortAlgo(arr,lo,p);
            quickSortAlgo(arr,p+1,hi);
        }
    }

    template <typename T>
    void quickSort(contiguous<T> &arr)
    {
        quickSortAlgo(*arr,1,arr.size());
    }

    template <typename T>
    static inline void testQS(randomized::bits &ran)
    {
        std::cerr << "testQS<" << typeid(T).name() << ">" << std::endl;
        for(size_t n=0;n<=512;++n)
        {
            vector<T> data(n,0);
            for(size_t iter=0;iter<64;++iter)
            {
                bring::fill(data,ran);
                //std::cerr << "raw=" << data << std::endl;
                quickSort(data);
                //std::cerr << "srt=" << data << std::endl;
                YACK_ASSERT(comparison::ordered(data,comparison::increasing<T>));
            }
        }
    }

}




YACK_UTEST(sort_quick)
{
    randomized::rand_ ran;

    testQS<int>(ran);
    testQS<double>(ran);

}
YACK_UDONE()

