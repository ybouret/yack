#include "yack/sort/network.hpp"
#include "yack/sort/nw/bosenelson3.hpp"
#include "yack/sort/nw/bosenelson2.hpp"
#include "yack/sort/nw/green16.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/type/v3d.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/wtime.hpp"
#include "../main.hpp"

using namespace yack;

template <typename T>
static inline void hs_perf(const size_t size, randomized::bits &ran)
{
    std::cerr << "Testing <HeapSort #" << size << ">" << std::endl;
    wtime     chrono;
    uint64_t  tmx = 0;
    double    ell = 0;
    size_t    num = 0;
    vector<T> v(size,as_capacity);

    do
    {
        v.free();
        for(size_t i=0;i<size;++i)
        {
            v << bring::get<T>(ran);
        }
        YACK_ASSERT(size==v.size());
        const uint64_t mark = wtime::ticks();
        hsort(v,comparison::increasing<T>);
        tmx += wtime::ticks() - mark;
        YACK_ASSERT(comparison::ordered(v,comparison::increasing<T>));
        ell = chrono(tmx);
        ++num;
    } while( ell < 0.5 );
    std::cerr << "rate=" << num/ell << std::endl;
}

template <typename T>
static inline
void nw_perf(const nwsrt::algorithm &algo, randomized::bits &ran)
{
    std::cerr << "Testing <" << algo.code.name << ">" << std::endl;

    const size_t    size = algo.code.size;
    vector<T>       v(size,as_capacity);
    vector<size_t>  u(size,as_capacity);

    wtime    chrono;
    uint64_t tmx = 0;
    double   ell = 0;
    size_t   num = 0;
    do
    {
        v.free();
        for(size_t i=0;i<size;++i)
        {
            v << bring::get<T>(ran);
        }
        YACK_ASSERT(size==v.size());
        const uint64_t mark = wtime::ticks();
        algo.increasing(v);
        tmx += wtime::ticks() - mark;
        YACK_ASSERT(comparison::ordered(v,comparison::increasing<T>));
        ell = chrono(tmx);
        ++num;
    } while( ell < 0.5 );
    std::cerr << "rate=" << num/ell << std::endl;
}

YACK_UTEST(sort_nw)
{
    randomized::rand_                 ran;

    hs_perf<int>(2,ran);

    nwsrt::agenda<nwsrt::bosenelson2> algo2;
    nw_perf<int>(algo2,ran);


    hs_perf<int>(16,ran);
    nwsrt::agenda<nwsrt::green16> algo16;
    nw_perf<int>(algo16,ran);

    
}
YACK_UDONE()




