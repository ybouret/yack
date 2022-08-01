
#include "yack/net/socket.hpp"
#include "yack/net/bsd.hpp"

namespace yack
{
    namespace net
    {
        socket:: ~socket() throw()
        {
            assert(invalid_socket!=sock);
            bsd::release(sock);
            assert(invalid_socket==sock);
        }

        socket:: socket(socket_type user_sock) throw() :
        sock(user_sock)
        {
            assert(invalid_socket!=user_sock);
        }

    }

}


