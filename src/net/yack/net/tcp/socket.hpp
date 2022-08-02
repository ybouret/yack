//! \file

#ifndef YACK_NET_TCP_SOCKET_INCLUDED
#define YACK_NET_TCP_SOCKET_INCLUDED 1

#include "yack/net/socket.hpp"
#include "yack/net/plexus.hpp"

namespace yack
{

    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! tcp socket
        //
        //______________________________________________________________________
        class tcp_socket : public socket
        {
        public:
            //! cleanup
            virtual ~tcp_socket() throw();

        protected:
            //! setup internal socket from plexus and address
            explicit tcp_socket(const plexus &, const socket_address &);

            //! setup internal socket from plexus and server socket
            explicit tcp_socket(const plexus &, const socket_type server_sock);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_socket);
        };
    }

}

#endif

