#include "yack/info/bwt.h"
#include "yack/sort/heap.h"
#include "yack/utest/run.hpp"
#include "yack/sequence/thin-array.hpp"

using namespace yack;



YACK_UTEST(info_bwt)
{
    float arr[] = { 2, 3, -1, 8 };
    thin_array<float> data(arr,sizeof(arr)/sizeof(arr[0]));
    std::cerr << data << std::endl;
    hpsort(sizeof(arr)/sizeof(arr[0]),arr-1);

    std::cerr << data << std::endl;

}
YACK_UDONE()


