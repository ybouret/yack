//! \file

#ifndef YACK_NET_TCP_SOCKET_INCLUDED
#define YACK_NET_TCP_SOCKET_INCLUDED 1

#include "yack/net/socket.hpp"

namespace yack
{

    namespace net
    {
        class tcp_socket : public socket
        {
        public:
            virtual ~tcp_socket() throw();

        protected:
            explicit tcp_socket(socket_type, const socket_address &) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_socket);
        };
    }

}

#endif

