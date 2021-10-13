
#include "yack/fft/xbitrev.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/type/utils.hpp"

using namespace yack;

namespace
{
    template <typename T,const size_t exp2>
    static inline
    void do_xtest()
    {
        uprng ran;
        memory::allocator &mgr = memory::global::instance();
        static const size_t size =  size_t(1) << exp2;
        size_t items = 4*size;
        size_t bytes = 0;
        T     *data  = mgr.allocate<T>(items,bytes);
        T     *wksp  = data+2*size;
        YACK_ASSERT(items>=4*size);

        for(size_t i=0;i<2*size;++i)
        {
            wksp[i] = data[i] = T( ran() );
        }
        fft::apply<T,exp2>(data-1,1);
        fft::apply<T,exp2>(data-1,-1);
        
        T sum2 = 0;
        for(size_t i=0;i<2*size;++i)
        {
            sum2 += square_of(wksp[i]-data[i]/size);
        }
        sum2 = sqrt(sum2);
        std::cerr << "sum2=" << sum2 << std::endl;
        mgr.withdraw(data,bytes);

    }

    template <const size_t exp2>
    static inline
    void do_xtests()
    {
        do_xtest<float,exp2>();
        do_xtest<double,exp2>();
    }

}

YACK_UTEST(fft_run)
{

    do_xtests<0>();
    do_xtests<1>();
    do_xtests<2>();
    do_xtests<3>();
    do_xtests<4>();
    do_xtests<5>();
    do_xtests<6>();
    do_xtests<7>();
    do_xtests<8>();
    do_xtests<9>();
    do_xtests<10>();
    do_xtests<11>();
    do_xtests<12>();
    do_xtests<13>();
    return 0;
    do_xtests<14>();
    do_xtests<15>();
    do_xtests<16>();
    do_xtests<17>();
    do_xtests<18>();
    do_xtests<19>();
    do_xtests<20>();
    do_xtests<21>();
    do_xtests<22>();

}
YACK_UDONE()
