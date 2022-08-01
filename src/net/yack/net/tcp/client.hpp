
//! \file

#ifndef YACK_NET_TCP_CLIENT_INCLUDED
#define YACK_NET_TCP_CLIENT_INCLUDED 1

#include "yack/net/tcp/socket.hpp"

namespace yack
{

    namespace net
    {

        class tcp_client
        {
        public:
            virtual ~tcp_client() throw();
            explicit tcp_client();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_client);
        };

    }

}

#endif

