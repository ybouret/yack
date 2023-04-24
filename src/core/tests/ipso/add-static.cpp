#include "yack/ipso/add-static.hpp"
#include "yack/utest/run.hpp"


using namespace yack;

YACK_UTEST(ipso_add_static)
{
    ipso::static_add<float,8> xadd;

    {
        static const int arr[5] = { -1, -2, -3, 10, 1000 };
        xadd.range(arr,sizeof(arr)/sizeof(arr[0]));
    }

    std::cerr << xadd.reduce() << std::endl;


}
YACK_UDONE()

