
#include "yack/net/com/socket.hpp"

namespace yack
{
    namespace net
    {

        com_socket:: ~com_socket() throw()
        {
        }


        com_socket:: com_socket(const socket_address &_, const socket_type __) throw():
        socket(_,__)
        {
        }

        com_socket:: com_socket(const plexus &_, const socket_type __) :
        socket(_,__)
        {
        }
        
        
    }

}

