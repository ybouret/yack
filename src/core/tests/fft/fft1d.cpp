
#include "yack/fft/fft1d.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/wtime.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/randomized/rand.hpp"

using namespace yack;

namespace
{

    static double rates[64];
    static size_t nrate=0;
    static double tsample = 0.5;

    template <typename T,const size_t exp2>
    static inline
    double do_xtest(randomized::bits &ran)
    {
        memory::allocator  &mgr = memory::global::instance();
        static const size_t size =  size_t(1) << exp2;
        std::cerr << "float" << sizeof(T)*8 << ", 2^" << std::setw(2) << exp2 << "=" << std::setw(6) << size << " :";
        size_t items = 4*size;
        size_t bytes = 0;
        T     *data  = mgr.allocate<T>(items,bytes);
        T     *wksp  = data+2*size;
        YACK_ASSERT(items>=4*size);
        
        for(size_t i=0;i<2*size;++i)
        {
            wksp[i] = data[i] = T( 100.0 * (ran()-0.5) );
        }
        
        
        fft1d::forward(data-1,size);
        fft1d::reverse(data-1,size);
        
        T sum2 = 0;
        for(size_t i=0;i<2*size;++i)
        {
            sum2 += squared(wksp[i]-data[i]/size);
        }
        sum2 = sqrt(sum2/size);
        std::cerr << " rms=" << std::setw(14) << sum2 << " :";

        double  rate = 0;
#if defined(YACK_FFT_TRACK)
        wtime  chrono;
        fft1d::algo_ticks     = 0;
        size_t iter           = 0;
        double ellapsed       = 0;
        do
        {
            ++iter;
            fft1d::forward(data-1,size);
            fft1d::reverse(data-1,size);
        } while( (ellapsed = chrono(fft1d::algo_ticks)) <= tsample );
        rate = 1e-3 * iter / ellapsed;
        std::cerr << " rate = "  << std::setw(14) << rate;
        if(exp2>=4 && sizeof(T)==sizeof(double))
        {
            rates[nrate++] = rate;
        }
#endif
        
        std::cerr << std::endl;
        
        mgr.withdraw(data,bytes);
        return rate;
    }
    
    
    static const char filename[] = "fft.dat";
    template <const size_t exp2>
    static inline
    void do_xtests(randomized::bits &ran)
    {
        const double frate = do_xtest<float,exp2>(ran);
        const double drate = do_xtest<double,exp2>(ran);

        ios::ocstream::echo(filename,"%u %g %g\n",unsigned(exp2),log10(frate),log10(drate));
    }
    
    template <typename T>
    static inline
    void displayPI()
    {
        YACK_SIZEOF(T);
        std::cerr.precision(200);
        const T PI=acos(T(-1));
        std::cerr << PI << std::endl;
    }
    
}

YACK_UTEST(fft1d)
{

    if(argc>1) tsample = atof(argv[1]);
    randomized::rand_  ran;
    ios::ocstream::overwrite(filename);

    do_xtests<0>(ran);
    do_xtests<1>(ran);
    do_xtests<2>(ran);
    do_xtests<3>(ran);
    do_xtests<4>(ran);
    do_xtests<5>(ran);
    do_xtests<6>(ran);
    do_xtests<7>(ran);
    do_xtests<8>(ran);
    do_xtests<9>(ran);
    do_xtests<10>(ran);
    do_xtests<11>(ran);
    do_xtests<12>(ran);
    do_xtests<13>(ran);
    do_xtests<14>(ran);

    if(false)
    {
        do_xtests<15>(ran);
        do_xtests<16>(ran);
        
        do_xtests<17>(ran);
        do_xtests<18>(ran);
        do_xtests<19>(ran);
        do_xtests<20>(ran);
        do_xtests<21>(ran);
        do_xtests<22>(ran);
    }


    {
        if(nrate)
        {
            
        }
    }

    if(false)
    {
        displayPI<float>();
        displayPI<double>();
        displayPI<long double>();
    }
    
    if(false)
    {
        size_t k=0;
        for(size_t i=0;i<8;++i)
        {
            for(size_t j=0;j<8;++j)
            {
                std::cout << " YFFT(" << std::setw(2) << k << "),";
                ++k;
            }
            std::cout << " \\" << std::endl;
        }
    }
    
}
YACK_UDONE()
