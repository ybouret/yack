

#include "yack/network.hpp"
#include "yack/net/socket/ipv4.hpp"
#include "yack/net/socket/ipv6.hpp"

#include "yack/utest/run.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/hexa.hpp"
#include <cstddef>
#include <cstring>

using namespace yack;


YACK_UTEST(addr)
{


    const uint16_t user_port = 7654;

    net::ipv4_address ip4; std::cerr << "ipv4@" << ip4.addr << std::endl;
    net::ipv6_address ip6; std::cerr << "ipv6@" << ip4.addr << std::endl;


    ip4 = net::inet_address_any;      ip4.port = YACK_NBO(user_port); std::cerr << ip4 << std::endl;
    ip4 = net::inet_address_none;     std::cerr << ip4 << std::endl;
    ip4 = net::inet_address_loopback; std::cerr << ip4 << std::endl;

    ip6 = net::inet_address_any;      ip6.port = YACK_NBO(user_port); std::cerr << ip6 << std::endl;
    ip6 = net::inet_address_none;     std::cerr << ip6 << std::endl;
    ip6 = net::inet_address_loopback; std::cerr << ip6 << std::endl;

}
YACK_UDONE()

