
#include "yack/net/socket/addr.hpp"

namespace yack
{
    namespace net
    {

        socket_addr:: ~socket_addr() throw()
        {
        }

        socket_addr:: socket_addr(sockaddr &ar, const sa_length_t sz, net16_t &pr) throw() :
        addr(ar),
        size(sz),
        port(pr)
        {
        }
        

    }

}
