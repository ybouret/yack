//! \file

#ifndef YACK_NET_UDP_SOCKET_INCLUDED
#define YACK_NET_UDP_SOCKET_INCLUDED 1

#include "yack/net/socket.hpp"
#include "yack/net/plexus.hpp"

namespace yack
{

    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! udp socket base class
        //
        //______________________________________________________________________
        class udp_socket : public socket
        {
        public:

            //! cleanup
            virtual ~udp_socket() throw();

            //! sendTo api
            void sendTo(const socket_address &dst,
                        const void           *msg,
                        const size_t          len,
                        const int             flags=0);

            //! recvFrom api
            size_t recvFrom(socket_address &src,
                            void           *msg,
                            const size_t    len,
                            const int       flags=0);

        protected:
            //! setup
            explicit udp_socket(const plexus &, const socket_address);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(udp_socket);
        };

 

    }

}

#endif

