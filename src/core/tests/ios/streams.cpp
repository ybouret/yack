
#include "yack/ios/ocstream.hpp"
#include "yack/ios/encoder.hpp"

#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_streams)
{

    ios::ocstream::overwrite("test.dat");
    ios::ocstream::echo("test.dat", "Hello from line %d\n", __LINE__);
    ios::ocstream::echo("test.dat", "World from line %d\n", __LINE__);

    {
        ios::ocstream fp("test2.dat");
        fp << 'a' << 'b' << 'c' << " def" << '\n';
        fp("result=%.3g\n", 3.14);
    }

    {
        const int8_t  x1  = 0xab;
        const int16_t x2  = 0xabcd;
        const int32_t x4  = 0x12345678;
        const int64_t x8  = YACK_U64(0x1234567887654321);
        size_t        nw  = 0;

        {
            ios::ocstream fp("test3.dat");
            nw += ios::encoder::emit(fp,x1);
            nw += ios::encoder::emit(fp,x2);
            nw += ios::encoder::emit(fp,x4);
            nw += ios::encoder::emit(fp,x8);
        }
        YACK_CHECK(nw==15);
    }

}
YACK_UDONE()

