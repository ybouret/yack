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

//--
#include "yack/sort/nw/bosenelson9.hpp"
#include "yack/sort/nw/batcher9.hpp"
#include "yack/sort/nw/hibbard9.hpp"
#include "yack/sort/nw/bitonic9.hpp"

#include "yack/sort/nw/bosenelson10.hpp"
#include "yack/sort/nw/batcher10.hpp"
#include "yack/sort/nw/hibbard10.hpp"
#include "yack/sort/nw/bitonic10.hpp"

#include "yack/sort/nw/bosenelson11.hpp"
#include "yack/sort/nw/batcher11.hpp"
#include "yack/sort/nw/hibbard11.hpp"
#include "yack/sort/nw/bitonic11.hpp"

#include "yack/sort/nw/bosenelson12.hpp"
#include "yack/sort/nw/batcher12.hpp"
#include "yack/sort/nw/hibbard12.hpp"
#include "yack/sort/nw/bitonic12.hpp"

#include "yack/sort/nw/bosenelson13.hpp"
#include "yack/sort/nw/batcher13.hpp"
#include "yack/sort/nw/hibbard13.hpp"
#include "yack/sort/nw/bitonic13.hpp"

#include "yack/sort/nw/bosenelson14.hpp"
#include "yack/sort/nw/batcher14.hpp"
#include "yack/sort/nw/hibbard14.hpp"
#include "yack/sort/nw/bitonic14.hpp"

#include "yack/sort/nw/bosenelson15.hpp"
#include "yack/sort/nw/batcher15.hpp"
#include "yack/sort/nw/hibbard15.hpp"
#include "yack/sort/nw/bitonic15.hpp"

#include "yack/sort/nw/bosenelson16.hpp"
#include "yack/sort/nw/batcher16.hpp"
#include "yack/sort/nw/hibbard16.hpp"
#include "yack/sort/nw/bitonic16.hpp"

#include "yack/sort/nw/bosenelson17.hpp"
#include "yack/sort/nw/batcher17.hpp"
#include "yack/sort/nw/hibbard17.hpp"
#include "yack/sort/nw/bitonic17.hpp"

#include "yack/sort/nw/bosenelson18.hpp"
#include "yack/sort/nw/batcher18.hpp"
#include "yack/sort/nw/hibbard18.hpp"
#include "yack/sort/nw/bitonic18.hpp"

#include "yack/sort/nw/bosenelson19.hpp"
#include "yack/sort/nw/batcher19.hpp"
#include "yack/sort/nw/hibbard19.hpp"
#include "yack/sort/nw/bitonic19.hpp"

#include "yack/sort/nw/bosenelson20.hpp"
#include "yack/sort/nw/batcher20.hpp"
#include "yack/sort/nw/hibbard20.hpp"
#include "yack/sort/nw/bitonic20.hpp"

#include "yack/sort/nw/bosenelson21.hpp"
#include "yack/sort/nw/batcher21.hpp"
#include "yack/sort/nw/hibbard21.hpp"
#include "yack/sort/nw/bitonic21.hpp"

#include "yack/sort/nw/bosenelson22.hpp"
#include "yack/sort/nw/batcher22.hpp"
#include "yack/sort/nw/hibbard22.hpp"
#include "yack/sort/nw/bitonic22.hpp"

#include "yack/sort/nw/bosenelson23.hpp"
#include "yack/sort/nw/batcher23.hpp"
#include "yack/sort/nw/hibbard23.hpp"
#include "yack/sort/nw/bitonic23.hpp"

#include "yack/sort/nw/bosenelson24.hpp"
#include "yack/sort/nw/batcher24.hpp"
#include "yack/sort/nw/hibbard24.hpp"
#include "yack/sort/nw/bitonic24.hpp"

//--

//----

#include "yack/sort/nw/floyd09.hpp"
#include "yack/sort/nw/senso09.hpp"

#include "yack/sort/nw/waksman10.hpp"
#include "yack/sort/nw/senso10.hpp"

#include "yack/sort/nw/shapirogreen11.hpp"
#include "yack/sort/nw/senso11.hpp"

#include "yack/sort/nw/shapirogreen12.hpp"
#include "yack/sort/nw/senso12.hpp"

#include "yack/sort/nw/end13.hpp"
#include "yack/sort/nw/senso13.hpp"

#include "yack/sort/nw/green14.hpp"
#include "yack/sort/nw/senso14.hpp"

#include "yack/sort/nw/green15.hpp"
#include "yack/sort/nw/senso15.hpp"

#include "yack/sort/nw/green16.hpp"
#include "yack/sort/nw/senso16.hpp"
#include "yack/sort/nw/vanvoorhis16.hpp"

#include "yack/sort/nw/sat17.hpp"
#include "yack/sort/nw/senso17.hpp"

#include "yack/sort/nw/alhajbaddar18.hpp"
#include "yack/sort/nw/senso18.hpp"

#include "yack/sort/nw/senso19.hpp"

#include "yack/sort/nw/sat20.hpp"
#include "yack/sort/nw/senso20.hpp"

#include "yack/sort/nw/senso21.hpp"

#include "yack/sort/nw/alhajbaddar22.hpp"
#include "yack/sort/nw/senso22.hpp"

#include "yack/sort/nw/senso23.hpp"
#include "yack/sort/nw/morwenn23.hpp"

#include "yack/sort/nw/morwenn24.hpp"


//----


#include "yack/sort/nw/bosenelson25.hpp"
#include "yack/sort/nw/batcher25.hpp"
#include "yack/sort/nw/hibbard25.hpp"
#include "yack/sort/nw/bitonic25.hpp"

#include "yack/sort/nw/bosenelson26.hpp"
#include "yack/sort/nw/batcher26.hpp"
#include "yack/sort/nw/hibbard26.hpp"
#include "yack/sort/nw/bitonic26.hpp"

#include "yack/sort/nw/bosenelson27.hpp"
#include "yack/sort/nw/batcher27.hpp"
#include "yack/sort/nw/hibbard27.hpp"
#include "yack/sort/nw/bitonic27.hpp"

#include "yack/sort/nw/bosenelson28.hpp"
#include "yack/sort/nw/batcher28.hpp"
#include "yack/sort/nw/hibbard28.hpp"
#include "yack/sort/nw/bitonic28.hpp"

#include "yack/sort/nw/bosenelson29.hpp"
#include "yack/sort/nw/batcher29.hpp"
#include "yack/sort/nw/hibbard29.hpp"
#include "yack/sort/nw/bitonic29.hpp"

#include "yack/sort/nw/bosenelson30.hpp"
#include "yack/sort/nw/batcher30.hpp"
#include "yack/sort/nw/hibbard30.hpp"
#include "yack/sort/nw/bitonic30.hpp"



#include "yack/sort/nw/bosenelson31.hpp"
#include "yack/sort/nw/batcher31.hpp"
#include "yack/sort/nw/hibbard31.hpp"
#include "yack/sort/nw/bitonic31.hpp"

#include "yack/sort/nw/bosenelson32.hpp"
#include "yack/sort/nw/batcher32.hpp"
#include "yack/sort/nw/hibbard32.hpp"
#include "yack/sort/nw/bitonic32.hpp"

#include "yack/sort/nw/green16.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/type/v3d.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/wtime.hpp"
#include "yack/system/rtti.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/string/ops.hpp"
#include "yack/hashing/sha1.hpp"
#include "yack/hashing/to.hpp"
#include "yack/hashing/md.hpp"
#include "yack/kr/digest.hpp"

#include "../main.hpp"

using namespace yack;

static double     duration = 0.5;

const char heap_sort[]  = "heap-sort.dat";


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
    const double rate= num/ell;
    std::cerr << rate << std::endl;
    ios::ocstream::echo(heap_sort,"%g %.15g\n",double(size),log10(rate));
}

template <typename T>
static inline
void nw_perf(const nwsrt::algorithm &algo, randomized::bits &ran)
{
    static const string &tid = rtti::name<T>();

    // preparing title
    string title = "[";
    title  += algo.code.name;
    title  += "<" + tid + ">] ";
    (std::cerr << std::setw(32) << title()).flush();

    // preparing data
    const size_t    size = algo.code.size;
    vector<T>       v(size,as_capacity);
    vector<size_t>  u(size,as_capacity);

    // timings
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
    const double rate = num/ell;
    std::cerr << rate << std::endl;

    // saving
    {
        string id = algo.code.name;
        strops::trim_if(isdigit,id);
        //std::cerr << "id=" << id << std::endl;
        id += ".dat";
        ios::ocstream::echo(id,"%g %.15g\n",double(size),log10(rate));
    }

    // checking co-sort
    v.free();
    u.free();
    vector<uint64_t> h(size,as_capacity);

    hashing::sha1 H;

    for(size_t i=0;i<size;++i)
    {
        H.set();
        v << bring::get<T>(ran);
        u << i;
        H.run(& v.back(), sizeof(T)      );
        H.run(& u.back(), sizeof(size_t) );
        h << hashing::to<uint64_t>(H);
    }
    YACK_ASSERT(size==h.size());
    hsort(h,comparison::increasing<uint64_t>);
    std::cerr << "v =" << v << std::endl;
    std::cerr << "u =" << u << std::endl;
    std::cerr << "h0=" << h << std::endl;
    const digest H0 = hashing::md::of(H, h(), size * sizeof(uint64_t) );

    algo.increasing(v,u);
    YACK_ASSERT(comparison::ordered(v,comparison::increasing<T>));

    for(size_t i=1;i<=size;++i)
    {
        H.set();
        H.run( & v[i], sizeof(T) );
        H.run( & u[i], sizeof(size_t) );
        h[i] = hashing::to<uint64_t>(H);
    }
    hsort(h,comparison::increasing<uint64_t>);
    std::cerr << "v =" << v << std::endl;
    std::cerr << "u =" << u << std::endl;
    std::cerr << "h1=" << h << std::endl;
    const digest H1 = hashing::md::of(H, h(), size * sizeof(uint64_t) );
    YACK_ASSERT(H0==H1);

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
    if(argc>1)
    {
        duration = ios::ascii::convert::real<double>(argv[1], "duration");
    }

    randomized::rand_                 ran;
    ios::ocstream::overwrite(heap_sort);
    ios::ocstream::overwrite("bosenelson.dat");
    ios::ocstream::overwrite("batcher.dat");
    ios::ocstream::overwrite("hibbard.dat");
    ios::ocstream::overwrite("bitonic.dat");
    ios::ocstream::overwrite("floyd.dat");
    ios::ocstream::overwrite("senso.dat");
    ios::ocstream::overwrite("waksman.dat");
    ios::ocstream::overwrite("shapirogreen.dat");
    ios::ocstream::overwrite("green.dat");
    ios::ocstream::overwrite("vanvoorhis.dat");
    ios::ocstream::overwrite("sat.dat");
    ios::ocstream::overwrite("morwenn.dat");
    ios::ocstream::overwrite("alhajbaddar.dat");

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

    NW_PERF_GENERIC(9);
    NW_PERF_GENERIC(10);
    NW_PERF_GENERIC(11);
    NW_PERF_GENERIC(12);
    NW_PERF_GENERIC(13);
    NW_PERF_GENERIC(14);
    NW_PERF_GENERIC(15);
    NW_PERF_GENERIC(16);
    NW_PERF_GENERIC(17);
    NW_PERF_GENERIC(18);
    NW_PERF_GENERIC(19);
    NW_PERF_GENERIC(20);
    NW_PERF_GENERIC(21);
    NW_PERF_GENERIC(22);
    NW_PERF_GENERIC(23);
    NW_PERF_GENERIC(24);


    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::floyd09> alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso09> alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::waksman10> alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso10>   alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::shapirogreen11> alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso11>        alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::shapirogreen12> alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso12>        alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::end13>    alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso13>  alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::green14>  alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso14>  alg; nw_perfs(alg,ran); }
    }


    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::green15>  alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso15>  alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::green16>       alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso16>       alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::vanvoorhis16>  alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::sat17>   alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso17> alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::alhajbaddar18>   alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso18>         alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::senso19> alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::sat20>   alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso20> alg; nw_perfs(alg,ran); }
    }


    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::senso21> alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::alhajbaddar22>   alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::senso22>         alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::senso23>     alg; nw_perfs(alg,ran); }
        { nwsrt::agenda<nwsrt::morwenn23>   alg; nw_perfs(alg,ran); }
    }

    {
        std::cerr << std::endl;
        { nwsrt::agenda<nwsrt::morwenn24>   alg; nw_perfs(alg,ran); }
    }


    NW_PERF_GENERIC(25);
    NW_PERF_GENERIC(26);
    NW_PERF_GENERIC(27);
    NW_PERF_GENERIC(28);
    NW_PERF_GENERIC(29);
    NW_PERF_GENERIC(30);
    NW_PERF_GENERIC(31);
    NW_PERF_GENERIC(32);

    std::cerr << "plot 'heap-sort.dat' w lp, 'batcher.dat' w lp, 'hibbard.dat' w lp, 'bitonic.dat' w lp, 'senso.dat' w lp, 'shapirogreen.dat' w lp, 'waksman.dat' w lp, 'green.dat' w lp, 'vanvoorhis.dat' w lp, 'sat.dat' w lp, 'alhajbaddar.dat' w lp, 'morwenn.dat' w lp, 'bosenelson.dat' w lp" << std::endl;

}
YACK_UDONE()




