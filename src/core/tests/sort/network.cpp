
#include "yack/sort/network/sort2.hpp"
#include "yack/sort/network/sort3.hpp"
#include "yack/sort/network/sort4.hpp"
#include "yack/sort/network/sort5.hpp"
#include "yack/sort/network/sort6.hpp"
#include "yack/sort/network/sort7.hpp"
#include "yack/sort/network/sort8.hpp"
#include "yack/sort/network/sort9.hpp"
#include "yack/sort/network/sort10.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

YACK_UTEST(sort_network)
{
    randomized::rand_ ran;
    network_sort2  nwsrt2;  YACK_CHECK(2==nwsrt2.size);
    network_sort3  nwsrt3;  YACK_CHECK(3==nwsrt3.size);
    network_sort4  nwsrt4;  YACK_CHECK(4==nwsrt4.size);
    network_sort5  nwsrt5;  YACK_CHECK(5==nwsrt5.size);
    network_sort6  nwsrt6;  YACK_CHECK(6==nwsrt6.size);
    network_sort7  nwsrt7;  YACK_CHECK(7==nwsrt7.size);
    network_sort8  nwsrt8;  YACK_CHECK(8==nwsrt8.size);
    network_sort9  nwsrt9;  YACK_CHECK(9==nwsrt9.size);
    network_sort10 nwsrt10; YACK_CHECK(10==nwsrt10.size);

    {
        v2d<double> v(1,-2);  std::cerr << v << std::endl;
        nwsrt2.increasing(v); std::cerr << v << std::endl;
    }

    {
        v3d<int> v(3,-1,2);   std::cerr << v << std::endl;
        nwsrt3.increasing(v); std::cerr << v << std::endl;
    }

    {
        double             xarr[4] = { 4,3,1,2 };
        double             yarr[4] = { 10,11,12,13};
        thin_array<double> xtab(xarr,4);
        thin_array<double> ytab(yarr,4);

        std::cerr << "x=" << xtab << std::endl;
        std::cerr << "y=" << ytab << std::endl;

        nwsrt4.increasing(xtab,ytab);
        std::cerr << "x=" << xtab << std::endl;
        std::cerr << "y=" << ytab << std::endl;
    }

    {
        matrix<uint32_t> m32(3,3);
        bring::fill(m32,ran);
        std::cerr << "m32=" << m32 << std::endl;
        {
            thin_array<uint32_t> a32 = m32.get_contiguous();
            nwsrt9.increasing(a32);
        }
        std::cerr << "m32=" << m32 << std::endl;

    }


    {
        vector<int> X(7,0);
        bring::fill(X,ran);
        std::cerr << "X7=" << X << std::endl;
        nwsrt7.decreasing_abs(X);
        std::cerr << "A7=" << X << std::endl;

    }

}
YACK_UDONE()


#include "yack/sort/network/sort.hpp"
#include "yack/comparison.hpp"
#include <typeinfo>

namespace {

    template <typename T> static inline
    void test_sum(const network_sort &nws, randomized::bits &ran)
    {
        std::cerr << "test_sum<" << typeid(T).name() <<  ">" << std::endl;
        for(size_t n=0;n<=100;++n)
        {
            vector<T> X(n,0);
            bring::fill(X,ran);
            {
                T         acc = 0;
                for(size_t i=1;i<=n;++i) acc += (X[i] = std::abs(X[i]));
                const T S = nws.sum_geqz(X);
                std::cerr << "S  =" << S << "/" << acc << " | delta=" << std::abs(S-acc) << std::endl;
            }

            {
                bring::fill(X,ran);
                T         acc = 0;
                for(size_t i=1;i<=n;++i)
                {
                    if(ran.choice()) X[i] = -X[i];
                    acc += X[i];
                }
                const T S = nws.sum_rand(X);
                std::cerr << "S  =" << S << "/" << acc << " | delta=" << std::abs(S-acc) << std::endl;
            }


        }
    }

}

YACK_UTEST(sort_nws)
{
    randomized::rand_ ran;
    const network_sort &nws = network_sort::instance();

    test_sum<int>(nws,ran);
    test_sum<float>(nws,ran);
    test_sum<double>(nws,ran);
    test_sum<long double>(nws,ran);

    std::cerr << "Testing All Sort" << std::endl;
    for(size_t n=0;n<=256;++n)
    {
        vector<int> xtab(n,0);
        for(size_t iter=0;iter<64;++iter)
        {
            bring::fill(xtab,ran);
            nws.increasing(xtab);
            YACK_ASSERT(comparison::ordered(xtab,comparison::increasing<int>));

            bring::fill(xtab,ran);
            nws.decreasing(xtab);
            YACK_ASSERT(comparison::ordered(xtab,comparison::decreasing<int>));

            bring::fill(xtab,ran);
            nws.decreasing_abs(xtab);
            YACK_ASSERT(comparison::ordered(xtab,comparison::decreasing_abs<int>));

        }
    }



}
YACK_UDONE()
