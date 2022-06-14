#include "yack/sort/network.hpp"

#include "yack/sort/nw/bosenelson2.hpp"
#include "yack/sort/nw/batcher2.hpp"
#include "yack/sort/nw/hibbard2.hpp"
#include "yack/sort/nw/bitonic2.hpp"

#include "yack/sort/nw/bosenelson3.hpp"
#include "yack/sort/nw/batcher3.hpp"
#include "yack/sort/nw/hibbard3.hpp"
#include "yack/sort/nw/bitonic3.hpp"

#include "yack/sort/nw/bosenelson4.hpp"
#include "yack/sort/nw/batcher4.hpp"
#include "yack/sort/nw/hibbard4.hpp"
#include "yack/sort/nw/bitonic4.hpp"


#include "yack/sort/nw/bosenelson5.hpp"
#include "yack/sort/nw/batcher5.hpp"
#include "yack/sort/nw/hibbard5.hpp"
#include "yack/sort/nw/bitonic5.hpp"

#include "yack/sort/nw/bosenelson6.hpp"
#include "yack/sort/nw/batcher6.hpp"
#include "yack/sort/nw/hibbard6.hpp"
#include "yack/sort/nw/bitonic6.hpp"

#include "yack/sort/nw/bosenelson7.hpp"
#include "yack/sort/nw/batcher7.hpp"
#include "yack/sort/nw/hibbard7.hpp"
#include "yack/sort/nw/bitonic7.hpp"

#include "yack/sort/nw/bosenelson8.hpp"
#include "yack/sort/nw/batcher8.hpp"
#include "yack/sort/nw/hibbard8.hpp"
#include "yack/sort/nw/bitonic8.hpp"

#include "yack/sort/nw/green16.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/type/v3d.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/wtime.hpp"
#include "yack/system/rtti.hpp"
#include "yack/ios/ocstream.hpp"

#include "../main.hpp"

using namespace yack;

static const char filename[] = "nw-perf.dat";
static double     duration = 0.5;

template <typename T>
static inline void hs_perf(const size_t size, randomized::bits &ran)
{
    static const string &tid = rtti::name<T>();
    string title = "[ hsort<" + tid + "> #"  + vformat("%u",unsigned(size)) + " ] ";
    (std::cerr << std::setw(32) << title()).flush();


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
    } while( ell < duration );
    std::cerr << num/ell << std::endl;
}

template <typename T>
static inline
void nw_perf(const nwsrt::algorithm &algo, randomized::bits &ran)
{
    static const string &tid = rtti::name<T>();
    string title = "[";
    title  += algo.code.name;
    title  += "<" + tid + ">] ";
    (std::cerr << std::setw(32) << title()).flush();

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
    } while( ell < duration );
    std::cerr << num/ell << std::endl;
}


static inline void hs_perfs(const size_t size, randomized::bits &ran)
{
    //hs_perf<int>(size,ran);
    hs_perf<double>(size,ran);
}

static inline void nw_perfs(const nwsrt::algorithm &algo, randomized::bits &ran)
{
    //nw_perf<int>(algo,ran);
    nw_perf<double>(algo,ran);
}

YACK_UTEST(sort_nw)
{
    randomized::rand_                 ran;
    ios::ocstream::overwrite(filename);

#define NW_PERF_GENERIC(N) do {                                      \
/**/ std::cerr << std::endl; \
/**/ hs_perfs(N,ran);                                                \
/**/ { nwsrt::agenda<nwsrt::bosenelson##N> alg; nw_perfs(alg,ran); } \
/**/ { nwsrt::agenda<nwsrt::batcher##N>    alg; nw_perfs(alg,ran); } \
/**/ { nwsrt::agenda<nwsrt::hibbard##N>    alg; nw_perfs(alg,ran); } \
/**/ { nwsrt::agenda<nwsrt::bitonic##N>    alg; nw_perfs(alg,ran); } \
} while(false)

    NW_PERF_GENERIC(2);
    NW_PERF_GENERIC(3);
    NW_PERF_GENERIC(4);
    NW_PERF_GENERIC(5);
    NW_PERF_GENERIC(6);
    NW_PERF_GENERIC(7);
    NW_PERF_GENERIC(8);

}
YACK_UDONE()




