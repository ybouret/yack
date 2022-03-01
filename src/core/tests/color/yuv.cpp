
#include "yack/color/yuv.hpp"
#include "yack/color/unit-real.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"

using namespace yack;

namespace
{
    template <typename T>
    static void test_yuv()
    {
        std::cerr << "yuv<" << rtti::name<T>() << ">" << std::endl;
        typedef color::nexus::yuv<T> YUV;

        for(size_t r=0;r<256;++r)
        {
            for(size_t g=0;g<256;++g)
            {
                for(size_t b=0;b<256;++b)
                {
                    const YUV     yuv = YUV::from_bytes(r,g,b);
                    const uint8_t R   = color::unit_real<T>::to_byte( yuv.get_r() );
                    const uint8_t G   = color::unit_real<T>::to_byte( yuv.get_g() );
                    const uint8_t B   = color::unit_real<T>::to_byte( yuv.get_b() );

                    YACK_ASSERT(R==r);
                    YACK_ASSERT(G==g);
                    YACK_ASSERT(B==b);

                }
            }
        }

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

