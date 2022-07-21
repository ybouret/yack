//!\file

#ifndef YACK_NET_SOCKET_ADDRESS_METRICS4_INCLUDED
#define YACK_NET_SOCKET_ADDRESS_METRICS4_INCLUDED 1

#include "yack/net/socket/address-metrics.hpp"

namespace yack
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! IPv4 address metrics
        //
        //______________________________________________________________________
        template <>
        struct socket_address_metrics<v4>
        {
            typedef sockaddr_in      sock_addr_type;                                    //!< alias
            typedef net32_t          inet_addr_type;                                    //!< alias
            static  const  size_t    sock_addr_size = sizeof(sock_addr_type);           //!< alias
            static  const  size_t    inet_addr_size = sizeof(inet_addr_type);           //!< alias
            static  const  ptrdiff_t addr_offset    = offsetof(sockaddr_in,sin_addr);   //!< offset of addr
            static  const  ptrdiff_t port_offset    = offsetof(sockaddr_in,sin_port);   //!< offset of port
            static  const  ptrdiff_t fmly_offset    = offsetof(sockaddr_in,sin_family); //!< offset of family

            static const uint16_t       family   = AF_INET;
            static const inet_addr_type any      = INADDR_ANY;      //!< alias
            static const inet_addr_type none     = INADDR_NONE;     //!< alias
            static const inet_addr_type loopback = INADDR_LOOPBACK; //!< alias

            static   void   initialize(sock_addr_type &) throw(); //!< set clean and family
            static   string to_string(const sock_addr_type &);    //!< convert address to string

        };

      

    }

}

#endif
