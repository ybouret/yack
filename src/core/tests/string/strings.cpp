#include "yack/string/string_.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(strings)
{
    const size_t bpc[] = {1,4};

    for(size_t i=0;i<=10;++i)
    {
        for(size_t j=0;j<2;++j)
        {
            kernel::string_ s(i,bpc[j]);
            std::cerr << "req=" << i << "*" << bpc[j] << " => width=" << s.width << std::endl;
        }
    }
}
YACK_UDONE()

