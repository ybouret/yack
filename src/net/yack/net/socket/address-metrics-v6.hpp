
//!\file

#ifndef YACK_NET_SOCKET_ADDRESS_METRICS6_INCLUDED
#define YACK_NET_SOCKET_ADDRESS_METRICS6_INCLUDED 1

#include "yack/net/socket/address-metrics.hpp"

namespace yack
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! IPv6 address metrics
        //
        //______________________________________________________________________
        template <>
        struct socket_address_metrics<v6>
        {
            typedef sockaddr_in6     sock_addr_type;                                      //!< alias
            typedef net128_t         inet_addr_type;                                      //!< alias
            static  const size_t     sock_addr_size = sizeof(sock_addr_type);             //!< alias
            static  const size_t     inet_addr_size = sizeof(inet_addr_type);             //!< alias
            static  const ptrdiff_t  addr_offset    = offsetof(sockaddr_in6,sin6_addr);   //!< offset of address
            static  const ptrdiff_t  port_offset    = offsetof(sockaddr_in6,sin6_port);   //!< offset of port
            static  const ptrdiff_t  fmly_offset    = offsetof(sockaddr_in6,sin6_family); //!< offset of family

            static const uint16_t       family   = AF_INET6;
            static const inet_addr_type any;      //!< alias
            static const inet_addr_type none;     //!< alias
            static const inet_addr_type loopback; //!< alias

            static void   initialize(sock_addr_type &) throw(); //!< set clean and family
            static string to_string(const sock_addr_type &);    //!< convert address to string

        };

   
    }

}

#endif
