
//! \file

#ifndef YACK_NET_TCP_CLIENT_INCLUDED
#define YACK_NET_TCP_CLIENT_INCLUDED 1

#include "yack/net/tcp/socket.hpp"

namespace yack
{

    namespace net
    {

        //______________________________________________________________________
        //
        //
        //! build server from plexus and address
        //
        //______________________________________________________________________
        class tcp_server_ : public tcp_socket
        {
        public:
            //! cleanup
            virtual ~tcp_server_() throw();

        protected:
            //! build tcp_socket and connect to address
            explicit tcp_server_(const plexus        &network,
                                 const socket_address srvaddr,
                                 const unsigned       pending);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_server_);
        };

        class tcp_server : public tcp_server_
        {
        public:
            virtual ~tcp_server() throw();

            explicit tcp_server(const plexus    &network,
                                const ip_version iplevel,
                                const uint16_t   srvport,
                                const unsigned   pending=1);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_server);
        };

    }

}

#endif
