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
        //! build client from plexus and address
        //
        //______________________________________________________________________
        class tcp_client_ : public tcp_socket
        {
        public:
            //! cleanup
            virtual ~tcp_client_() throw();

        protected:
            //! build tcp_socket and connect to address
            explicit tcp_client_(const plexus &, const socket_address);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_client_);
        };

        //______________________________________________________________________
        //
        //
        //! build client from plexus and different arguments
        //
        //______________________________________________________________________
        class tcp_client : public tcp_client_
        {
        public:
            //__________________________________________________________________
            //
            //C++
            //__________________________________________________________________

            //! cleanup
            virtual ~tcp_client() throw();

            //! setup from plexus and address
            explicit tcp_client(const plexus &, const socket_address);

            //! setup from plexus+hostName+port+version
            template <typename HOSTNAME> inline
            explicit tcp_client(const plexus    &network,
                                const HOSTNAME  &hostName,
                                const uint16_t   port,
                                const ip_version version) :
            tcp_client_(network, network.resolve(hostName,port,version) )
            {
            }

            //! setup from plexus+'name:port'+version
            template <typename FULLNAME> inline
            explicit tcp_client(const plexus    &network,
                                const FULLNAME  &fullName,
                                const ip_version version) :
            tcp_client_(network, network.resolve(fullName,version) )
            {
            }
            


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tcp_client);
        };

    }

}

#endif

