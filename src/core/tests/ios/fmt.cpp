#include "yack/ios/fmt/hexa.hpp"
#include "yack/ios/fmt/binary.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/ios/encoder.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_fmt)
{

    {
        ios::ocstream fp("fmt.dat");
        ios::ostream &os = fp;
        {
            uint8_t a = 0; std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x0a;      std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x12;      std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
        }

        {
            uint16_t a = 0; std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x0a;       std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x12;       std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x123;      std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x1234;     std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';

        }

        {
            uint32_t a = 0; std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x0a;       std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x12;       std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x123;      std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x1234;     std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x12345;    std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x123456;   std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x1234567;  std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
            a = 0x12345678; std::cerr << ios::hexa(a) << std::endl; os << ios::hexa(a) << '\n';
        }
        std::cerr << std::endl;

        {
            uint8_t a = 0; std::cerr << ios::binary(a) << std::endl; os << ios::binary(a) << '\n';
            a = 0x01;      std::cerr << ios::binary(a) << std::endl; os << ios::binary(a) << '\n';
            a = 0x02;      std::cerr << ios::binary(a) << std::endl; os << ios::binary(a) << '\n';
            a = 0xab;      std::cerr << ios::binary(a) << std::endl; os << ios::binary(a) << '\n';
            a = 0x80;      std::cerr << ios::binary(a) << std::endl; os << ios::binary(a) << '\n';
        }
    }

    {
        const int     n = 10;
        ios::ocstream fp(ios::cstderr);

        size_t nw = ios::encoder::addr2hexa(fp,&n);
        fp << '\n';
        std::cerr << "Written #" << nw << std::endl;


    }

}
YACK_UDONE()


