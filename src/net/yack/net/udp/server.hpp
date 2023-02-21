
//! \file

#ifndef YACK_NET_UDP_SERVER_INCLUDED
#define YACK_NET_UDP_SERVER_INCLUDED 1

#include "yack/net/udp/socket.hpp"

namespace yack
{

    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! udp server
        //
        //______________________________________________________________________
        class udp_server : public udp_socket
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~udp_server() noexcept;

            //! setup
            explicit udp_server(const plexus    &network,
                                const ip_version iplevel,
                                const uint16_t   srvport);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(udp_server);
        };

    }

}

#endif
