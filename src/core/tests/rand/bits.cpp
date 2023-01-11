
#include "yack/randomized/rand.hpp"
#include "yack/randomized/park-miller.hpp"
#include "yack/randomized/mt19937.hpp"

#include "yack/randomized/shuffle.hpp"
#include "yack/randomized/in2d.hpp"
#include "yack/randomized/in3d.hpp"
#include "yack/randomized/gaussian.hpp"

#include "yack/utest/run.hpp"
#include "yack/sequence/stats.hpp"
#include "yack/ios/fmt/hexa.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"
#include "yack/system/seed.hpp"

#include <cstring>

using namespace yack;

namespace
{

    template <typename T>
    static inline void draw( randomized::bits &ran )
    {
        std::cerr << "uint" << sizeof(T)*8 << "_t:";
        for(size_t i=0;i<4;++i)
        {
            std::cerr << ' ' << ios::hexa( ran.to<T>() );
        }
        std::cerr << std::endl;
    }

    template <typename SEQUENCE> static inline
    void do_stats( SEQUENCE &seq, const char *kind )
    {
        typedef typename SEQUENCE::mutable_type type;
        math::adder<type> xadd;
        std::cerr << "<" << kind << "/" << rtti::name<type>() << ">" << std::endl;
        type ave = statistical::average<type>::of(seq,xadd);
        std::cerr << "\taverage = " << ave << std::endl;
        type sig = sqrt( statistical::variance<type>::of(seq,ave,xadd) );
        std::cerr << "\tstd_dev = " << sig << std::endl;


    }

    static void test_bits( randomized::shared_bits &sh )
    {
        randomized::bits &ran = *sh;
        draw<uint8_t>(ran);
        draw<uint16_t>(ran);
        draw<uint32_t>(ran);
        draw<uint64_t>(ran);

        const size_t   N = 10000;
        vector<float>  xf(N,as_capacity);
        vector<double> xd(N,as_capacity);
        for(size_t i=0;i<N;++i)
        {
            xf.push_back( ran.to<float>()  );
            xd.push_back( ran.to<double>() );
        }

        do_stats(xf,"uniform");
        do_stats(xd,"uniform");

        xf.free();
        xd.free();
        randomized::gaussian<float>  gf(sh);
        randomized::gaussian<double> gd(sh);
        for(size_t i=0;i<N;++i)
        {
            xf.push_back( gf()  );
            xd.push_back( gd() );
        }
        do_stats(xf,"gaussian");
        do_stats(xd,"gaussian");


        xf.free();
        xd.free();
        for(size_t i=0;i<N;++i)
        {
            xf.push_back( ran.symm<float>()  );
            xd.push_back( ran.symm<double>() );
        }
        do_stats(xf,"symmetric");
        do_stats(xd,"symmetric");

        vector<long double>               hsphere(5);
        randomized::gaussian<long double> ge(sh);

        ge(hsphere);
        std::cerr << "hsphere: " << hsphere << std::endl;

        size_t np=0,nn=0;
        for(size_t i=0;i<N;++i)
        {
            if( ran.choice() ) ++np; else ++nn;
        }
        std::cerr << "choice: " << np/double(N) << " | " << nn/double(N) << std::endl;

    }

}


YACK_UTEST(rand_bits)
{



    {
        randomized::shared_bits  ran   = new randomized::rand_();
        randomized::shared_bits  ranPM = new randomized::ParkMiller();
        randomized::shared_bits  mt    = new randomized::mt19937();

        std::cerr << "Testing rand()" << std::endl;
        test_bits(ran);

        std::cerr << "Testing Park-Miller" << std::endl;
        test_bits(ranPM);

        std::cerr << "Testing mt19937" << std::endl;
        test_bits(mt);
    }


    std::cerr << "seeds: " << std::endl;
    for(size_t i=0;i<10;++i)
    {
        std::cerr << ios::hexa( system_seed::get64() ) << std::endl;
    }
}
YACK_UDONE()


