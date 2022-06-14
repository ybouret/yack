
#include "yack/sort/network/sort2.hpp"
#include "yack/sort/network/sort3.hpp"
#include "yack/sort/network/sort4.hpp"
#include "yack/sort/network/sort9.hpp"

#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

YACK_UTEST(sort_network)
{
    randomized::rand_ ran;
    network_sort2 nwsrt2; YACK_CHECK(2==nwsrt2.size);
    network_sort3 nwsrt3; YACK_CHECK(3==nwsrt3.size);
    network_sort4 nwsrt4; YACK_CHECK(4==nwsrt4.size);
    network_sort9 nwsrt9; YACK_CHECK(9==nwsrt9.size);

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


}
YACK_UDONE()


