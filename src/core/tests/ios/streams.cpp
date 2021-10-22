
#include "yack/ios/ocstream.hpp"
#include "yack/ios/encoder.hpp"

#include "yack/ios/icstream.hpp"
#include "yack/ios/decoder.hpp"

#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    static inline void test_userialize(const uint64_t x)
    {
        size_t nw = 0;
        {
            ios::ocstream fp("test-pack.dat");
            nw = ios::encoder::serialize(fp,x);
            //std::cerr << "sent " << x << std::endl;
        }

        {
            ios::icstream fp("test-pack.dat");
            uint64_t y = 0;
            const size_t  nr = ios::decoder::construct(fp,y,"y");
            //std::cerr << "recv " << y << std::endl;
            YACK_ASSERT(nr==nw);
            YACK_ASSERT(x==y);
        }
    }
}

YACK_UTEST(ios_streams)
{

    randomized::rand_ ran;

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
            YACK_CHECK(nw==15);


        }

        {
            ios::icstream input("test3.dat");
            int8_t y1 = 0;
            YACK_CHECK(1==ios::decoder::read(input,y1));
            YACK_CHECK(x1==y1);

            int16_t y2 = 0;
            YACK_CHECK(2==ios::decoder::read(input,y2));
            YACK_CHECK(x2==y2);

            int32_t y4 = 0;
            YACK_CHECK(4==ios::decoder::read(input,y4));
            YACK_CHECK(x4==y4);


            int64_t y8 = 0;
            YACK_CHECK(8==ios::decoder::read(input,y8));
            YACK_CHECK(x8==y8);

        }
    }


    {
        std::cerr << "Dumping.." << std::endl;

        ios::icstream fp("test2.dat");
        char C = 0;
        while(fp.query(C))
        {
            fp.store(C);
            YACK_ASSERT(fp.query(C));
            std::cerr << C;
        }
    }

    std::cerr << "[pack/unpack]" << std::endl;
    test_userialize(0);
    for(size_t bits=0;bits<=64;++bits)
    {
        for(size_t iter=0;iter<16;++iter)
        {
            const uint64_t x = ran.gen<uint64_t>(bits);
            YACK_ASSERT(bits_for(x)==bits);
            test_userialize(x);
        }
    }
    test_userialize(integral_for<uint64_t>::maximum);


    if(false)
    {
        ios::icstream fp(ios::cstdin);
        char C = 0;
        while(fp.query(C))
        {
            fp.store(C);
            YACK_ASSERT(fp.query(C));
            std::cerr << C;
        }
    }


}
YACK_UDONE()

