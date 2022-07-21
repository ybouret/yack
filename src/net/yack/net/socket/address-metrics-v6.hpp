
//!\file

#ifndef YACK_NET_SOCKET_ADDRESS_METRICS6_INCLUDED
#define YACK_NET_SOCKET_ADDRESS_METRICS6_INCLUDED 1

#include "yack/net/socket/address-metrics.hpp"

namespace yack
{
    namespace net
    {
        template <>
        struct socket_address_metrics<v6>
        {
            typedef sockaddr_in6     sock_addr_type;
            typedef net128_t         inet_addr_type;
            static  const size_t     sock_addr_size = sizeof(sock_addr_type);
            static  const size_t     inet_addr_size = sizeof(inet_addr_type);
            static  const ptrdiff_t  inet_offset    = offsetof(sockaddr_in6,sin6_addr);
            static  const ptrdiff_t  port_offset    = offsetof(sockaddr_in6,sin6_port);

            static const inet_addr_type any;
            static const inet_addr_type none;
            static const inet_addr_type loopback;

            static void   initialize(sock_addr_type &) throw();
            static string to_string(const sock_addr_type &);

        };

   
    }

}

#endif
