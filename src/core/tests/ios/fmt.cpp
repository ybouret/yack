#include "yack/ios/fmt/hexa.hpp"
#include "yack/ios/fmt/binary.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_fmt)
{

    {
        uint8_t a = 0; std::cerr << ios::hexa(a) << std::endl;
        a = 0x0a;      std::cerr << ios::hexa(a) << std::endl;
        a = 0x12;      std::cerr << ios::hexa(a) << std::endl;
    }

    {
        uint16_t a = 0; std::cerr << ios::hexa(a) << std::endl;
        a = 0x0a;       std::cerr << ios::hexa(a) << std::endl;
        a = 0x12;       std::cerr << ios::hexa(a) << std::endl;
        a = 0x123;      std::cerr << ios::hexa(a) << std::endl;
        a = 0x1234;     std::cerr << ios::hexa(a) << std::endl;

    }

    {
        uint32_t a = 0; std::cerr << ios::hexa(a) << std::endl;
        a = 0x0a;       std::cerr << ios::hexa(a) << std::endl;
        a = 0x12;       std::cerr << ios::hexa(a) << std::endl;
        a = 0x123;      std::cerr << ios::hexa(a) << std::endl;
        a = 0x1234;     std::cerr << ios::hexa(a) << std::endl;
        a = 0x12345;    std::cerr << ios::hexa(a) << std::endl;
        a = 0x123456;   std::cerr << ios::hexa(a) << std::endl;
        a = 0x1234567;  std::cerr << ios::hexa(a) << std::endl;
        a = 0x12345678; std::cerr << ios::hexa(a) << std::endl;
    }
    std::cerr << std::endl;

    {
        uint8_t a = 0; std::cerr << ios::binary(a) << std::endl;
        a = 0x01;      std::cerr << ios::binary(a) << std::endl;
        a = 0x02;      std::cerr << ios::binary(a) << std::endl;
        a = 0xab;      std::cerr << ios::binary(a) << std::endl;
        a = 0x80;      std::cerr << ios::binary(a) << std::endl;

    }

}
YACK_UDONE()


