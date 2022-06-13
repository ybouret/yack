#include "yack/sort/network.hpp"
#include "yack/sort/nw/bosenelson3.hpp"
#include "yack/sort/nw/green16.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/type/v3d.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

template <typename T>
static inline
void nw(const nwsrt::swaps &swp, randomized::bits &ran)
{
    const size_t size = swp.size;
    vector<T>    v(size,as_capacity);
    for(size_t iter=0;iter<1;++iter)
    {
        v.free();
        for(size_t i=0;i<size;++i)
        {
            v << bring::get<T>(ran);
        }
        YACK_ASSERT(size==v.size());
        std::cerr << "raw=" << v << std::endl;
        network_sort::increasing(v,swp);
        std::cerr << "srt=" << v << std::endl;
    }
}

YACK_UTEST(sort_nw)
{
    randomized::rand_        ran;
    const nwsrt::bosenelson3 swp3;
    const nwsrt::green16     swp16;
    nw<int>(swp3,ran);
    nw<apq>(swp3,ran);
    
    nw<unsigned>(swp16,ran);
    
    
}
YACK_UDONE()




