
#include "yack/ios/ocstream.hpp"
#include "yack/ios/null-ostream.hpp"
#include "yack/memory/allocator/legacy.hpp"
#include "yack/arith/base2.hpp"

#include "yack/ios/encoder.hpp"

#include "yack/ios/icstream.hpp"
#include "yack/ios/decoder.hpp"
#include "yack/ios/imstream.hpp"

#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

namespace
{
    static inline void test_userialize(const uint64_t x)
    {
        size_t nw = 0;
        {
            ios::ocstream fp("test-pack.dat");
            nw = ios::encoder::serialize64(fp,x);
        }

        {
            ios::icstream fp("test-pack.dat");
            uint64_t      y = 0;
            const size_t  nr = ios::decoder::construct64(fp,y,"y");
            YACK_ASSERT(nr==nw);
            YACK_ASSERT(x==y);
        }

        const size_t bits = bits_for(x);
        if(bits<=32)
        {
            ios::icstream fp("test-pack.dat");
            uint32_t     dw=0;
            const size_t nr = ios::decoder::construct(fp,dw,"dw");
            YACK_ASSERT(nr==nw);
            YACK_ASSERT(x==dw);
        }

        if(bits<=16)
        {
            ios::icstream fp("test-pack.dat");
            uint16_t     w=0;
            const size_t nr = ios::decoder::construct(fp,w,"w");
            YACK_ASSERT(nr==nw);
            YACK_ASSERT(x==w);
        }

        if(bits<=8)
        {
            ios::icstream fp("test-pack.dat");
            uint8_t      b=0;
            const size_t nr = ios::decoder::construct(fp,b,"b");
            YACK_ASSERT(nr==nw);
            YACK_ASSERT(x==b);
        }

    }

    template <typename T>
    static inline void test_ser()
    {
        static const T arr[] = {
            integral_for<T>::minimum,
            integral_for<T>::minimum/2,
            0,
            integral_for<T>::maximum/2,
            integral_for<T>::maximum };
        static const size_t num = sizeof(arr)/sizeof(arr[0]);

        for(size_t i=0;i<num;++i)
        {
            size_t nw = 0;
            {
                ios::ocstream fp("test-pack.dat");
                nw = ios::encoder::serialize(fp,arr[i]);
            }

            {
                ios::icstream fp("test-pack.dat");
                T      y = 0;
                const size_t  nr = ios::decoder::construct(fp,y,"y");
                YACK_ASSERT(nr==nw);
                YACK_ASSERT(arr[i]==y);
            }
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

    std::cerr << "[serialize64/construct64]" << std::endl;
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

    std::cerr << "[serialize/construct]" << std::endl;
    test_ser<int8_t>();
    test_ser<uint8_t>();
    test_ser<int16_t>();
    test_ser<uint16_t>();
    test_ser<int32_t>();
    test_ser<uint32_t>();
    test_ser<int64_t>();
    test_ser<uint64_t>();

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

    {
        ios::null_ostream fp;
        const size_t nw = fp("Hello");
        std::cerr << "nw.null=" << nw << std::endl;
    }

    {
        const char   msg[] = "Hello, World!!";
        const size_t len   = strlen(msg);

        size_t nw = 0;
        {
            ios::ocstream fp("bin.dat");
            nw = ios::encoder::serialize(fp,msg,len);
        }
        std::cerr << "binary written=" << nw << std::endl;

        {
            ios::icstream fp("bin.dat");
            void  *block_addr = 0;
            size_t block_size = 0;
            const size_t nr = ios::decoder::construct(fp,block_addr,block_size,"message");
            std::cerr << "binary read=" << nr << std::endl;

            YACK_CHECK(nr==nw);
            YACK_CHECK(len==block_size);
            YACK_CHECK(0==memcmp(msg,block_addr,block_size));
            memory::legacy::release(block_addr);
        }

        {
            std::cerr << "imstream: [";
            ios::imstream fp(msg,len);
            char C;
            while( fp.query(C) )
            {
                std::cerr << C;
            }
            std::cerr << "]" << std::endl;
        }

    }


    YACK_SIZEOF(ios::istream);
    YACK_SIZEOF(ios::icstream);
    YACK_SIZEOF(ios::imstream);

    


}
YACK_UDONE()

