
#include "yack/network.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(init)
{
    net::network::verbose = true;
    net::network &nw = net::network::instance();

    YACK_CHECK(net::v4 == nw.ip_version_from("v4") );
    YACK_CHECK(net::v6 == nw.ip_version_from("v6") );

    
}
YACK_UDONE()

