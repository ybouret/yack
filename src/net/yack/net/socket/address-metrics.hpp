
//! \file

#ifndef YACK_NET_SOCKET_ADDRESS_METRICS_INCLUDED
#define YACK_NET_SOCKET_ADDRESS_METRICS_INCLUDED 1

#include "yack/net/types.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{
    namespace net
    {

        //______________________________________________________________________
        //
        //! forward declaration
        //______________________________________________________________________
        template <const  ip_version> struct socket_address_metrics;

        //______________________________________________________________________
        //
        //! ops for socket_address
        //______________________________________________________________________
        struct socket_address_ops
        {
            static void add_dec_to(string &, const uint8_t); //!< add decimal to string
        };

      

    }

}

#endif
