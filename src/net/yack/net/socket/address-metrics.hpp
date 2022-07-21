
//! \file

#ifndef YACK_NET_SOCKET_ADDRESS_METRICS_INCLUDED
#define YACK_NET_SOCKET_ADDRESS_METRICS_INCLUDED 1

#include "yack/net/types.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{
    namespace net
    {
        template <const  ip_version>
        struct socket_address_metrics;

        struct socket_address_ops
        {
            static void add_dec_to(string &, const uint8_t);
        };

        enum inet_address_name
        {
            inet_address_any,
            inet_address_none,
            inet_address_loopback
        };

    }

}

#endif
