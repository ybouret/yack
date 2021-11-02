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

    void *rra[1];

    vector<float> data(100,as_capacity);
    for(size_t n=1+ran.leq(20);n>0;--n)
    {
        data.push_back( ran.to<float>()-0.5f );
    }

    std::cerr << "raw:" <<  data << std::endl;
    yack_heap_sort(*data,data.size(),sizeof(float),rra,cmp<float>,NULL);

    std::cerr << "srt:" << data << std::endl;
    YACK_CHECK(comparison::ordered(&data[1],data.size(), comparison::increasing<float> ));

}
YACK_UDONE()


