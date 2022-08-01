#include "yack/net/tcp/client.hpp"

namespace yack
{

    namespace net
    {
        tcp_client:: ~tcp_client() throw()
        {
        }

        tcp_client:: tcp_client(const plexus &network, const socket_address client_addr) :
        tcp_socket(network.tcp_client_socket(client_addr),client_addr)
        {

        }
        

    }

}
