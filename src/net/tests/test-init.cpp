
#include "yack/network.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(init)
{
    network::verbose = true;
    network &nw = network::instance();

    YACK_CHECK(net::v4 == nw.ip_version("v4") );
    YACK_CHECK(net::v6 == nw.ip_version("v6") );

    
}
YACK_UDONE()

