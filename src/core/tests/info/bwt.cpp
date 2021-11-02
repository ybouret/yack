#include "yack/info/bwt.h"
#include "yack/sort/heap.h"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/comparison.hpp"

using namespace yack;

template <typename T>
static inline int cmp(const void *lhs, const void *rhs, void *)
{
    return comparison::increasing<T>( *static_cast<const T*>(lhs), *static_cast<const T*>(rhs) );
}

YACK_UTEST(info_bwt)
{
    randomized::rand_ ran;

    vector<float> data(100,as_capacity);
    for(size_t n=1+ran.leq(20);n>0;--n)
    {
        data.push_back( ran.to<float>() );
    }

    std::cerr << "raw:" <<  data << std::endl;
    hpsort(data.size(),*data,cmp<float>,NULL);

    std::cerr << "srt:" << data << std::endl;
    YACK_CHECK(comparison::ordered(&data[1],data.size(), comparison::increasing<float> ));

}
YACK_UDONE()


