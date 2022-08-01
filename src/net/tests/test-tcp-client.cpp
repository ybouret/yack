
#include "yack/net/tcp/client.hpp"
#include "yack/net/plexus.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(tcp_client)
{
    net::plexus::verbose = true;
    net::plexus &network = net::plexus::instance();

    if(argc>1) {
        net::socket_address ip = network.resolve(argv[1],net::v4);
    }
}
YACK_UDONE()

