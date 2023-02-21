
#include "yack/net/socket.hpp"
#include "yack/net/bsd.hpp"

namespace yack
{
    namespace net
    {
        socket:: ~socket() noexcept
        {
            assert(invalid_socket!=sock);
            on_quit();
            bsd::release( coerce(sock) );
            assert(invalid_socket==sock);
        }

        socket:: socket(const socket_address  user_addr,
                        const socket_type     user_sock) noexcept :
        self(user_addr),
        sock(user_sock),
        skey(sock)
        {
            assert(invalid_socket!=user_sock);
            on_init();
        }

        socket::const_inward &socket:: bulk() const noexcept { return self; }

        const socket_key &socket::key() const noexcept
        {
            return skey;
        }

        void socket::shutdown_recv() noexcept
        {
            bsd::closure(sock,sd_recv);
        }

        void socket::shutdown_send() noexcept
        {
            bsd::closure(sock,sd_send);
        }

        void socket::shutdown_both() noexcept
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
        sock( network.accept(server_sock,coerce(self) ) ),
        skey(sock)
        {
            on_init();
        }


        void socket:: on_init() const noexcept
        {
            if(plexus::verbose)
            {
                std::cerr << "[created socket <" << self << ">]" << std::endl;
            }
        }


        void socket:: on_quit() const noexcept
        {
            if(plexus::verbose)
            {
                std::cerr << "[deleted socket <" << self << ">]" << std::endl;
            }
        }
    }
}

