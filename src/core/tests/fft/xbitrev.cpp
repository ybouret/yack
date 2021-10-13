#include "yack/fft/xbitrev.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"

using namespace yack;

namespace
{
    template <typename T,const size_t exp2>
    static inline
    void do_xtest()
    {
        memory::allocator &mgr = memory::global::instance();
        static const size_t size =  size_t(1) << exp2;
        size_t items = 2*size;
        size_t bytes = 0;
        T     *data  = mgr.allocate<T>(items,bytes);
        YACK_ASSERT(items>=2*size);

        fft::bitrev<exp2>::run(data-1);

        mgr.withdraw(data,bytes);

    }

    template <const size_t exp2>
    static inline
    void do_xtests()
    {
        static const size_t size = size_t(1) << exp2;
        do_xtest<float,exp2>();
        do_xtest<double,exp2>();
        std::cerr << "exp2=" << exp2 << " => " << "size=" << size << " => " << fft::bitrev_count<exp2>() << std::endl;
    }

}

YACK_UTEST(fft_xbitrev)
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

