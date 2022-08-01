#include "yack/net/tcp/socket.hpp"

namespace yack
{

    namespace net
    {


        tcp_socket:: ~tcp_socket() throw()
        {
        }

        tcp_socket:: tcp_socket(const socket_type s, const socket_address &a) throw() :
        socket(s,a)
        {
        }
        

    }

}
