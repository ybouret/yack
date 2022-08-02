#include "yack/net/udp/socket.hpp"

namespace yack
{

    namespace net
    {

        udp_socket_:: ~udp_socket_() throw() {}

        udp_socket_:: udp_socket_(const plexus &network, const socket_address user_addr) :
        socket(user_addr,network.open_udp(user_addr.version()) )
        {
        }
        
    }

}
namespace yack
{

    namespace net
    {

        udp_socket:: ~udp_socket() throw() {}

        udp_socket:: udp_socket(const plexus &network, const socket_address user_addr) :
        udp_socket_(network,user_addr)
        {
        }

    }

}
