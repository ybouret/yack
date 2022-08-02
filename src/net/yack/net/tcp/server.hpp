
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
            explicit tcp_server_(const plexus &, const socket_address);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_server_);
        };

    }

}

#endif
