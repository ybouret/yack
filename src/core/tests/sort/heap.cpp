#include "yack/sort/heap.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/randomized/shuffle.hpp"
#include "yack/comparison.hpp"

using namespace yack;

YACK_UTEST(sort_heap)
{
    randomized::rand_ ran;
    
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
YACK_UDONE()


