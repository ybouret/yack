
#include "yack/fft/fft1d.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/type/utils.hpp"

using namespace yack;

namespace
{
    template <typename T,const size_t exp2>
    static inline
    void do_xtest(uprng &ran)
    {
        memory::allocator  &mgr = memory::global::instance();
        static const size_t size =  size_t(1) << exp2;
        std::cerr << "float" << sizeof(T)*8 << ", 2^" << exp2 << "=" << size << " : " << std::endl;
        size_t items = 4*size;
        size_t bytes = 0;
        T     *data  = mgr.allocate<T>(items,bytes);
        T     *wksp  = data+2*size;
        YACK_ASSERT(items>=4*size);

        for(size_t i=0;i<2*size;++i)
        {
            wksp[i] = data[i] = T( ran() );
        }

        fft1d::apply<T,exp2>(data-1,1);
        fft1d::apply<T,exp2>(data-1,-1);
        
        T sum2 = 0;
        for(size_t i=0;i<2*size;++i)
        {
            sum2 += square_of(wksp[i]-data[i]/size);
        }
        sum2 = sqrt(sum2);
        std::cerr << "\tsum2=" << sum2 << std::endl;
        mgr.withdraw(data,bytes);

    }

    template <const size_t exp2>
    static inline
    void do_xtests(uprng &ran)
    {
        do_xtest<float,exp2>(ran);
        do_xtest<double,exp2>(ran);
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
    uprng ran;

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
    if(false)
    {
    do_xtests<14>(ran);
    do_xtests<15>(ran);
    do_xtests<16>(ran);
    do_xtests<17>(ran);
    do_xtests<18>(ran);
    do_xtests<19>(ran);
    do_xtests<20>(ran);
    do_xtests<21>(ran);
    do_xtests<22>(ran);
    }

    displayPI<float>();
    displayPI<double>();
    displayPI<long double>();

}
YACK_UDONE()
