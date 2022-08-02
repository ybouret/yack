

#include "yack/net/tcp/server.hpp"
#include "yack/net/plexus.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(tcp_server)
{
    net::plexus::verbose = true;
    net::plexus &network = net::plexus::instance();
    std::cerr << program << " on " << network.hostname << std::endl;
}
YACK_UDONE()

