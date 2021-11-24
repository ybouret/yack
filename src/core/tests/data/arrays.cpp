#include "yack/sequence/arrays.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

YACK_UTEST(data_arrays)
{
    randomized::rand_ ran;

    YACK_SIZEOF(thin_array<double>);
    YACK_SIZEOF(thin_array<int>);


    for(size_t num_arrays=0;num_arrays<=10;++num_arrays)
    {
        for(size_t num_blocks=0;num_blocks<=100;++num_blocks)
        {
            arrays_of<char>   iarr(num_arrays,num_blocks);
            arrays_of<double> darr(num_arrays,num_blocks);
            arrays_of<string> sarr(num_arrays,num_blocks);

        }
    }



#if 0
    arrays_of<double> arr(10,32);
    for(size_t i=1;i<=arr.count;++i)
    {
        writable<double> &a = arr[i];
        std::cerr << "a[" << i << "].size=" << a.size() << std::endl;
        for(size_t j=1;j<=a.size();++j)
        {
            a[j] = ran.symm<double>();
        }
    }
#endif
    
}
YACK_UDONE()

