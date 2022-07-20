

#include "yack/network.hpp"
#include "yack/net/bsd.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    static inline
    void play_with(net::socket_type &s)
    {

        const int recvBufferSize = net::bsd::getopt<int>(s,SOL_SOCKET,SO_RCVBUF);
        const int sendBufferSize = net::bsd::getopt<int>(s,SOL_SOCKET,SO_SNDBUF);
        const int reUseAddress   = net::bsd::getopt<int>(s,SOL_SOCKET,SO_REUSEADDR);
        std::cerr << "recvBufferSize   = " << recvBufferSize << std::endl;
        std::cerr << "sendBufferSize   = " << sendBufferSize << std::endl;
        std::cerr << "reUseAddress     = " << reUseAddress   << std::endl;

        const int reUse = 1;
        net::bsd::setopt(s,SOL_SOCKET,SO_REUSEADDR,reUse);
        std::cerr << "reUseAddress (*) = " << net::bsd::getopt<int>(s,SOL_SOCKET,SO_REUSEADDR) << std::endl;

        net::bsd::set_blocking(s,false);

    }

}


YACK_UTEST(socket)
{
    network::verbose = true;
    network &nw = network::instance();
    std::cerr << "on " << nw.hostname << std::endl;

    net::socket_type s4 = net::bsd::acquire(net::v4,net::tcp);

    play_with(s4);

    net::bsd::closure(s4,net::sd_send);
    net::bsd::closure(s4,net::sd_recv);
    net::bsd::closure(s4,net::sd_both);

    net::bsd::release(s4);


}
YACK_UDONE()

