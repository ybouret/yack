
//! \file

#ifndef YACK_NET_TCP_SERVER_INCLUDED
#define YACK_NET_TCP_SERVER_INCLUDED 1

#include "yack/net/tcp/socket.hpp"

namespace yack
{

    namespace net
    {
        class tcp_client_;

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
            virtual ~tcp_server_() noexcept;

        protected:
            //! build tcp_socket and connect to address
            explicit tcp_server_(const plexus        &network,
                                 const socket_address srvaddr,
                                 const unsigned       pending);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_server_);
        };

        //______________________________________________________________________
        //
        //
        //! build tcp server
        //
        //______________________________________________________________________
        class tcp_server : public tcp_server_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~tcp_server() noexcept;

            //! setup
            explicit tcp_server(const plexus    &network,
                                const ip_version iplevel,
                                const uint16_t   srvport,
                                const unsigned   pending=1);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_server);
            friend class tcp_client_;
        };

    }

}

#endif
