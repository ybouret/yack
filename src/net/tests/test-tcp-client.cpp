
#include "yack/net/tcp/client.hpp"
#include "yack/network.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(tcp_client)
{
    net::network &Net = net::network::instance();
    if(argc>1)
    {
        net::socket_address ip = Net.resolve(argv[1],net::v4);
    }
}
YACK_UDONE()

