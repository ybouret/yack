

#include "yack/net/plexus.hpp"
#include "yack/net/socket/address.hpp"
#include "yack/utest/run.hpp"

using namespace yack;


YACK_UTEST(addr)
{


    const uint16_t user_port = 7654;

    net::IPv4 ip4; std::cerr << "ipv4@" << ip4.addr << std::endl;
    net::IPv6 ip6; std::cerr << "ipv6@" << ip4.addr << std::endl;


    ip4 = net::inet_address_any;      ip4.port = YACK_NBO(user_port); std::cerr << ip4 << std::endl;
    ip4 = net::inet_address_none;     std::cerr << ip4 << std::endl;
    ip4 = net::inet_address_loopback; std::cerr << ip4 << std::endl;

    ip6 = net::inet_address_any;      ip6.port = YACK_NBO(user_port); std::cerr << ip6 << std::endl;
    ip6 = net::inet_address_none;     std::cerr << ip6 << std::endl;
    ip6 = net::inet_address_loopback; std::cerr << ip6 << std::endl;


    net::socket_address sa4a( net::v4, net::inet_address_loopback, 1234);
    net::socket_address sa4b(sa4a);
    net::socket_address sa6a( net::v6, net::inet_address_loopback, 5678);
    net::socket_address sa6b(sa6a);

    std::cerr << "sa4a = " << sa4a << std::endl;
    std::cerr << "sa4b = " << sa4b << std::endl;
    std::cerr << "sa6a = " << sa6a << std::endl;
    std::cerr << "sa6b = " << sa6b << std::endl;

    net::socket_address sa(net::v4);
    sa = sa4b;
    std::cerr << "sa: " << sa << std::endl;
    sa = sa6b;
    std::cerr << "sa: " << sa << std::endl;


}
YACK_UDONE()

