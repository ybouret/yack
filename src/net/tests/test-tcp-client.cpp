
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
        const string          fullName = argv[1];
        const net::ip_version version  = (argc>2) ? network.ip_version_from(argv[2]) : net::v4;
        net::tcp_client client(network,fullName,version);
        std::cerr << "tcp_client " << *client << ",key=" << client.key() << std::endl;
    }
}
YACK_UDONE()

