
#include "yack/net/plexus.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(init)
{
    net::plexus::verbose = true;
    net::plexus &network = net::plexus::instance();

    YACK_CHECK(net::v4 == network.ip_version_from("v4") );
    YACK_CHECK(net::v6 == network.ip_version_from("v6") );

    
}
YACK_UDONE()

