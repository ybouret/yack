

#include "yack/network.hpp"
#include "yack/net/socket/address-metrics-v4.hpp"
#include "yack/net/socket/address-metrics-v6.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/hexa.hpp"
#include <cstddef>
#include <cstring>

using namespace yack;

namespace yack
{
    namespace net
    {
        


#define YACK_NET_SOCKET_ADDR_CTOR() sa(),                                             \
addr( out_of_reach::access<inet_addr_type,sock_addr_type>(sa,metrics::inet_offset) ), \
port( out_of_reach::access<net16_t,       sock_addr_type>(sa,metrics::port_offset) )

        template <const ip_version ipv>
        class socket_addr : public object
        {
        public:
            typedef socket_address_metrics<ipv>       metrics;
            typedef typename metrics::sock_addr_type  sock_addr_type;
            typedef typename metrics::inet_addr_type  inet_addr_type;
            static const size_t                       sock_addr_size = metrics::sock_addr_size;
            static const size_t                       inet_addr_size = metrics::inet_addr_size;

            socket_addr() throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                metrics::initialize(sa);
            }

            socket_addr(const inet_address_name id) throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                metrics::initialize(sa);
                switch(id)
                {
                    case inet_address_any:      out_of_reach::copy(addr,metrics::any);      break;
                    case inet_address_none:     out_of_reach::copy(addr,metrics::none);     break;
                    case inet_address_loopback: out_of_reach::copy(addr,metrics::loopback); break;
                }
                (void) endian::BEaddr(&addr,inet_addr_size);
            }

            virtual ~socket_addr() throw()
            {
                metrics::initialize(sa);
            }

            socket_addr(const socket_addr &other) throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                memcpy(&sa,&other.sa,sock_addr_size);
            }

            socket_addr & operator=( const socket_addr &other ) throw()
            {
                memmove(&sa,&other.sa,sock_addr_size);
                return *this;
            }

        public:
            sock_addr_type     sa;
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

    ipv4 = net::inet_address_any;      std::cerr << net::socket_address_metrics<net::v4>::to_string(ipv4.sa) << std::endl;
    ipv4 = net::inet_address_none;     std::cerr << net::socket_address_metrics<net::v4>::to_string(ipv4.sa) << std::endl;
    ipv4 = net::inet_address_loopback; std::cerr << net::socket_address_metrics<net::v4>::to_string(ipv4.sa) << std::endl;

    ipv6 = net::inet_address_any;      std::cerr << net::socket_address_metrics<net::v6>::to_string(ipv6.sa) << std::endl;
    ipv6 = net::inet_address_none;     std::cerr << net::socket_address_metrics<net::v6>::to_string(ipv6.sa) << std::endl;
    ipv6 = net::inet_address_loopback; std::cerr << net::socket_address_metrics<net::v6>::to_string(ipv6.sa) << std::endl;


}
YACK_UDONE()

