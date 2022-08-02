
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
        class udp_socket_ : public socket
        {
        public:
            //! cleanup
            virtual ~udp_socket_() throw();

        protected:
            //! setup internal socket from plexus and address
            explicit udp_socket_(const plexus &, const socket_address );


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(udp_socket_);
        };


        //______________________________________________________________________
        //
        //
        //! udp socket
        //
        //______________________________________________________________________
        class udp_socket : public udp_socket_
        {
        public:
            virtual ~udp_socket() throw();
            explicit udp_socket(const plexus &, const socket_address);

            //! setup from plexus+hostName+port+version
            template <typename HOSTNAME> inline
            explicit udp_socket(const plexus    &network,
                                const HOSTNAME  &hostName,
                                const uint16_t   port,
                                const ip_version version) :
            udp_socket_(network, network.resolve(hostName,port,version) )
            {
            }

            //! setup from plexus+'name:port'+version
            template <typename FULLNAME> inline
            explicit udp_socket(const plexus    &network,
                                const FULLNAME  &fullName,
                                const ip_version version) :
            udp_socket_(network, network.resolve(fullName,version) )
            {
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(udp_socket);
        };

    }

}

#endif

