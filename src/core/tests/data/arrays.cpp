#include "yack/sequence/arrays.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

namespace {

    template <typename T>
    static inline void test_arrays(const size_t      num_arrays,
                                   const size_t      num_blocks,
                                   randomized::bits &ran)
    {
        arrays_of<T> arr(num_arrays,num_blocks);
        std::cerr << "#arr=" << arr.size() << std::endl;
        std::cerr << "    |_mutual_size: " << arr.mutual_size() << std::endl;
        std::cerr << "    |_fixed_bytes: " << arr.fixed_bytes() << std::endl;
        std::cerr << "    |_granted    : " << arr.granted()     << std::endl;

        for(size_t i=arr.size();i>0;--i)
        {
            writable<T> &a = arr[i];
            YACK_ASSERT(a.size() == arr.mutual_size());
            for(size_t j=a.size();j>0;--j)
            {
                const T tmp = bring::get<T>(ran);
                a[j] = tmp;
            }
        }
        arr.make(num_blocks*2);
        std::cerr << "    |_mutual_size: " << arr.mutual_size() << std::endl;
        std::cerr << "    |_fixed_bytes: " << arr.fixed_bytes() << std::endl;
        std::cerr << "    |_granted    : " << arr.granted()     << std::endl;

        arr.release();
    }
}

YACK_UTEST(data_arrays)
{
    randomized::rand_ ran;

    YACK_SIZEOF(thin_array<double>);
    YACK_SIZEOF(thin_array<int>);


    for(size_t num_arrays=0;num_arrays<=10;++num_arrays)
    {
        for(size_t num_blocks=0;num_blocks<=100;++num_blocks)
        {
            test_arrays<char>(num_arrays,num_blocks,ran);
            test_arrays<double>(num_arrays,num_blocks,ran);
            test_arrays<string>(num_arrays,num_blocks,ran);
        }
    }

    
    
}
YACK_UDONE()

