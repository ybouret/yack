
//! \file

#ifndef YACK_NET_TCP_CLIENT_INCLUDED
#define YACK_NET_TCP_CLIENT_INCLUDED 1

#include "yack/net/tcp/socket.hpp"
#include "yack/net/plexus.hpp"

namespace yack
{

    namespace net
    {

        
        class tcp_client : public tcp_socket
        {
        public:
            virtual ~tcp_client() throw();
            explicit tcp_client(const plexus &, const socket_address);
            



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_client);
        };

    }

}

#endif

