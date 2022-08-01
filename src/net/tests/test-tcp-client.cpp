
#include "yack/net/tcp/client.hpp"
#include "yack/net/plexus.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(tcp_client)
{
    net::plexus::verbose = true;
    net::plexus &network = net::plexus::instance();

    if(argc>1)
    {
        const string    fullName = argv[1];
        net::ip_version version  = net::v4;
        if(argc>2)      version = network.ip_version_from(argv[2]);
        net::tcp_client client(network,fullName,version);
    }
}
YACK_UDONE()

