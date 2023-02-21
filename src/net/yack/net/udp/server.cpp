
#include "yack/net/udp/server.hpp"
#include "yack/net/bsd.hpp"

namespace yack
{

    namespace net
    {

        udp_server:: ~udp_server() noexcept
        {
        }

        udp_server:: udp_server(const plexus    &network,
                                const ip_version iplevel,
                                const uint16_t   srvport) :
        udp_socket(network, socket_address(iplevel,inet_address_any,srvport))
        {
            bsd::_bind(sock,self->addr,self->size );
        }

    }
}
