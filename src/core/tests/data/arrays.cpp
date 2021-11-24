#include "yack/sequence/arrays.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_arrays)
{
    randomized::rand_ ran;

    YACK_SIZEOF(thin_array<double>);
    YACK_SIZEOF(thin_array<int>);

    { arrays arr(0); }
    { arrays arr(1); }
    { arrays arr(10); }
    { arrays arr(100); }
    { arrays arr(1000); }

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

