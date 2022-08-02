#include "yack/net/tcp/socket.hpp"

namespace yack
{

    namespace net
    {


        tcp_socket:: ~tcp_socket() throw()
        {
        }

        tcp_socket:: tcp_socket(const plexus &network, const socket_address &user_addr) :
        socket( user_addr, network.open_tcp(user_addr.version()) )
        {
            
        }
        

    }

}


namespace yack
{

    namespace net
    {

        tcp_socket:: tcp_socket(const plexus &network, const socket_type server_sock) :
        socket(network,server_sock)
        {
        }


    }

}
