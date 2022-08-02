#include "yack/net/tcp/client.hpp"

namespace yack
{
    namespace net
    {
        tcp_client_:: ~tcp_client_() throw()
        {
        }

        tcp_client_:: tcp_client_(const plexus &network, const socket_address client_addr) :
        tcp_socket(network,client_addr)
        {
            network.connect(sock,self);
        }



    }

    namespace net
    {
        tcp_client:: ~tcp_client() throw()
        {
        }

        tcp_client:: tcp_client(const plexus &network, const socket_address client_addr) :
        tcp_client_(network,client_addr)
        {
        }
    }




}

#include "yack/net/tcp/server.hpp"

namespace yack
{
    namespace net
    {

        tcp_client_:: tcp_client_(const plexus &network, const tcp_server &srv) :
        tcp_socket(network,srv.sock)
        {

        }

        tcp_client:: tcp_client(const plexus &network, const tcp_server &srv) :
        tcp_client_(network,srv)
        {

        }
    }

}
