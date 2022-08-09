#include "yack/net/comm/buffer.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(buffer)
{
    net::comm::buffer buff(100);

    std::cerr << "#bytes=" << buff.bytes << std::endl;

}
YACK_UDONE()




