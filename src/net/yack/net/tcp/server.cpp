
#include "yack/net/tcp/server.hpp"

namespace yack
{
    namespace net
    {
        tcp_server_:: ~tcp_server_() throw()
        {
        }

        tcp_server_:: tcp_server_(const plexus        &network,
                                  const socket_address srvaddr,
                                  const unsigned       pending) :
        tcp_socket(network,srvaddr)
        {
            network.tcp_bind(sock,self);
            network.tcp_listen(sock,self,pending);
        }

    }

}

namespace yack
{
    namespace net
    {

        tcp_server:: ~tcp_server() throw()
        {
        }

        tcp_server:: tcp_server(const plexus    &network,
                                const ip_version iplevel,
                                const uint16_t   srvport,
                                const unsigned   pending) :
        tcp_server_(network, socket_address(iplevel,inet_address_any,srvport), pending )
        {
        }
        
    }

}
