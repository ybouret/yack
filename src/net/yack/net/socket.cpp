
#include "yack/net/socket.hpp"
#include "yack/net/bsd.hpp"

namespace yack
{
    namespace net
    {
        socket:: ~socket() throw()
        {
            assert(invalid_socket!=sock);
            bsd::release( coerce(sock) );
            assert(invalid_socket==sock);
        }

        socket:: socket(socket_type user_sock,
                        const socket_address &user_addr) throw() :
        sock(user_sock),
        self(user_addr)
        {
            assert(invalid_socket!=user_sock);
        }

        socket::const_type &socket:: bulk() const throw() { return self; }


    }

}


