
#include "yack/color/yuv.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    template <typename T>
    static void test_yuv()
    {
        const T y1 = color::nexus::yuv<T>::to_y(1,1,1);
        std::cerr << "y1=" << y1 << std::endl;

        color::nexus::yuv<T> YUV;
    }
}

YACK_UTEST(color_yuv)
{
    color::yuv  c;

    test_yuv<float>();
    test_yuv<double>();
    test_yuv<long double>();

}
YACK_UDONE()

