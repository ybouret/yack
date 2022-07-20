

#include "yack/network.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/out-of-reach.hpp"
#include <cstddef>
#include <cstring>

using namespace yack;

namespace yack
{
    namespace net
    {
        template <const  ip_version>
        struct socket_address_metrics;

        template <>
        struct socket_address_metrics<v4>
        {
            typedef sockaddr_in  sock_addr_type;
            typedef net32_t      inet_addr_type;
            static const size_t  inet_offset = offsetof(sockaddr_in,sin_addr);
            static const size_t  port_offset = offsetof(sockaddr_in,sin_port);
        };

        template <>
        struct socket_address_metrics<v6>
        {
            typedef sockaddr_in6 sock_addr_type;
            typedef net128_t     inet_addr_type;
            static const size_t  inet_offset = offsetof(sockaddr_in6,sin6_addr);
            static const size_t  port_offset = offsetof(sockaddr_in6,sin6_port);
        };

#define YACK_NET_SOCKET_ADDR_CTOR() \
sock_addr(), \
addr( out_of_reach::access<inet_addr_type,sock_addr_type>(sock_addr,metrics::inet_offset) ),\
port( out_of_reach::access<net16_t,       sock_addr_type>(sock_addr,metrics::port_offset) )

        template <const ip_version ipv>
        class socket_addr
        {
        public:
            typedef socket_address_metrics<ipv>       metrics;
            typedef typename metrics::sock_addr_type  sock_addr_type;
            typedef typename metrics::inet_addr_type  inet_addr_type;

            explicit socket_addr() throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
            }

            virtual ~socket_addr() throw()
            {
            }

        private:
            sock_addr_type sock_addr;

            void clear() throw() { memset(&sock_addr,0,sizeof(sock_addr)); }

        public:
            inet_addr_type    &addr;
            net16_t           &port;
        };




    }

}


YACK_UTEST(addr)
{
    sockaddr_in  a4;
    sockaddr_in6 a6;
    memset(&a4,0,sizeof(a4));
    memset(&a6,0,sizeof(a6));

    YACK_SIZEOF(sockaddr_in);
    YACK_SIZEOF(sockaddr_in6);



    const unsigned sin4_port_offset = offsetof(sockaddr_in,sin_port);
    const unsigned sin6_port_offset = offsetof(sockaddr_in6,sin6_port);
    const unsigned sin4_addr_offset = offsetof(sockaddr_in,sin_addr);
    const unsigned sin6_addr_offset = offsetof(sockaddr_in6,sin6_addr);

    std::cerr << "sin4_port_offset=" << sin4_port_offset << std::endl;
    std::cerr << "sin4_addr_offset=" << sin4_addr_offset << std::endl;

    std::cerr << "sin6_port_offset=" << sin6_port_offset << std::endl;
    std::cerr << "sin6_addr_offset=" << sin6_addr_offset << std::endl;


    net::socket_addr<net::v4> ipv4; std::cerr << "ipv4@" << ipv4.addr << std::endl;
    net::socket_addr<net::v6> ipv6; std::cerr << "ipv6@" << ipv6.addr << std::endl;

    YACK_SIZEOF(net::socket_addr<net::v4>);
    YACK_SIZEOF(net::socket_addr<net::v6>);

}
YACK_UDONE()

