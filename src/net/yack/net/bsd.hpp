
//! \file

#ifndef YACK_NET_BSD_INCLUDED
#define YACK_NET_BSD_INCLUDED 1

#include "yack/net/types.hpp"

namespace yack
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! BSD sockets API
        //
        //______________________________________________________________________
        struct bsd
        {
            //__________________________________________________________________
            //
            //! shutdow operations
            //__________________________________________________________________
            enum sd_how
            {
                sd_send, //!< shutdown send
                sd_recv, //!< shutdown recv
                sd_both  //!< shutdown both
            };

            static socket_type acquire(const ip_version, const ip_protocol);  //!< open socket
            static void        release(socket_type &)                throw(); //!< close socket
            static void        perform(socket_type &, const sd_how)  throw(); //!< perform shutdown
        };
    }
}
#endif

