//! \file

#ifndef YACK_NET_UDP_CLIENT_INCLUDED
#define YACK_NET_UDP_CLIENT_INCLUDED 1

#include "yack/net/udp/socket.hpp"
#include "yack/net/plexus.hpp"

namespace yack
{

    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! udp client
        //
        //______________________________________________________________________
        class udp_client : public udp_socket
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~udp_client() throw();

            //! setup directly
            explicit udp_client(const plexus       &,
                                const socket_address);

            //! setup from plexus+hostName+port+version
            template <typename HOSTNAME> inline
            explicit udp_client(const plexus    &network,
                                const HOSTNAME  &hostName,
                                const uint16_t   port,
                                const ip_version version) :
            udp_socket(network, network.resolve(hostName,port,version) )
            {
            }

            //! setup from plexus+'name:port'+version
            template <typename FULLNAME> inline
            explicit udp_client(const plexus    &network,
                                const FULLNAME  &fullName,
                                const ip_version version) :
            udp_socket(network, network.resolve(fullName,version) )
            {
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(udp_client);
        };

    }

}

#endif
