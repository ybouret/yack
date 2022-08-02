
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

        void socket::shutdown_recv() throw()
        {
            bsd::closure(sock,sd_recv);
        }

        void socket::shutdown_send() throw()
        {
            bsd::closure(sock,sd_send);
        }

        void socket::shutdown_both() throw()
        {
            bsd::closure(sock,sd_both);
        }

        void socket:: blocking(bool flag)
        {
            bsd::set_blocking(sock,flag);
        }


        size_t socket:: recv_buffer() const
        {
            const int res = net::bsd::getopt<int>(sock,SOL_SOCKET,SO_RCVBUF);
            if(res<0) throw yack::exception("recv_buffer<0");
            return res;
        }

        size_t socket:: send_buffer() const
        {
            const int res = net::bsd::getopt<int>(sock,SOL_SOCKET,SO_SNDBUF);
            if(res<0) throw yack::exception("send_buffer<0");
            return res;
        }


    }

}


#include "yack/net/plexus.hpp"

namespace yack
{
    namespace net
    {
        socket:: socket(const plexus &network, const socket_type server_sock) :
        self(v4),
        sock( network.tcp_accept(server_sock,coerce(self) ) ),
        skey(sock)
        {
            
        }
    }
}

