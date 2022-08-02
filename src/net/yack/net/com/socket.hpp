

#ifndef YACK_NET_COM_SOCKET_INCLUDED
#define YACK_NET_COM_SOCKET_INCLUDED 1

#include "yack/net/socket.hpp"
#include "yack/net/plexus.hpp"

namespace yack
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! com socket
        //
        //______________________________________________________________________
        class com_socket : public socket
        {
        public:
            //! cleanup
            virtual ~com_socket() throw();

        protected:
            //! forward to socket
            explicit com_socket(const socket_address &, const socket_type) throw();

            //! forward from tcp_client
            explicit com_socket(const plexus &, const socket_type server_sock);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(com_socket);
        };
    }

}

#endif

