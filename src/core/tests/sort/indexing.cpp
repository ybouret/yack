
#include "yack/sort/indexing.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/randomized/shuffle.hpp"
#include "yack/comparison.hpp"
#include "yack/sequence/list.hpp"

using namespace yack;

YACK_UTEST(sort_indexing)
{
    randomized::rand_  ran;
    int                arr[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    const size_t       num = sizeof(arr)/sizeof(arr[0]);
    size_t             pos[num] = { 0 };
    thin_array<int>    vec(arr,num);
    thin_array<size_t> idx(pos,num);
    list<int>          out(num,0);

    randomized::shuffle::data(arr, num, ran);
    indexing::make(idx,comparison::increasing<int>,vec);
    indexing::rank(out,vec,idx);

    std::cerr << "vec=" << vec << std::endl;
    std::cerr << "idx=" << idx << std::endl;
    std::cerr << "out=" << out << std::endl;


    //YACK_CHECK(comparison::ordered(arr,num,comparison::increasing<int>));
    //randomized::shuffle::data(arr, num, ran);
    //hsort(vec,idx,comparison::increasing<int> );
    //YACK_CHECK(comparison::ordered(arr,num,comparison::increasing<int>));

}
YACK_UDONE()


