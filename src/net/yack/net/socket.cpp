
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

        socket:: socket(const socket_address &user_addr,
                        socket_type           user_sock) throw() :
        self(user_addr),
        sock(user_sock),
        skey(sock)
        {
            assert(invalid_socket!=user_sock);
        }

        socket::const_type &socket:: bulk() const throw() { return self; }

        const socket_key &socket::key() const throw()
        {
            return skey;
        }

    }

}


