#include "yack/sort/heap.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/randomized/shuffle.hpp"
#include "yack/comparison.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sort/heap.h"
#include <typeinfo>

using namespace yack;

template <typename T> static inline
int legacy_compare(const void *lhs, const void *rhs, void *) throw()
{
    return comparison::increasing( *(const T*)lhs, *(const T *)rhs);
}

template <typename T>
static inline void test_heap_sort( randomized::bits &ran )
{
    std::cerr << "heap sort with " << typeid(T).name() << std::endl;
    const size_t nmax = 1000;
    vector<T>    data(nmax,as_capacity);

    void *wksp[ YACK_WORDS_FOR(T) ];
    for(size_t n=0;n<=nmax;++n)
    {
        data.free();
        for(size_t i=0;i<n;++i) data.push_back( ran.to<T>() );
        randomized::shuffle::data(*data+1,data.size(),ran);
        yack_heap_sort(*data,data.size(),sizeof(T),wksp,legacy_compare<T>,NULL);
        YACK_ASSERT(comparison::ordered(*data+1,data.size(),comparison::increasing<T>));
    }


}


YACK_UTEST(sort_heap)
{
    randomized::rand_ ran;

    {
        int                arr[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        size_t             pos[] = { 1,2,3,4,5,6,7,8 };
        const size_t       num = sizeof(arr)/sizeof(arr[0]);
        thin_array<int>    vec(arr,num);
        thin_array<size_t> idx(pos,num);

        randomized::shuffle::data(arr, num, ran);
        std::cerr << "raw=" << vec << std::endl;
        hsort(vec, comparison::increasing<int> );
        std::cerr << "srt=" << vec << std::endl;

        YACK_CHECK(comparison::ordered(arr,num,comparison::increasing<int>));
        randomized::shuffle::data(arr, num, ran);
        hsort(vec,idx,comparison::increasing<int> );
        YACK_CHECK(comparison::ordered(arr,num,comparison::increasing<int>));
    }

    test_heap_sort<float>(ran);
    test_heap_sort<double>(ran);

}
YACK_UDONE()


