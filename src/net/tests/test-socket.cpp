

#include "yack/network.hpp"
#include "yack/utest/run.hpp"

#if defined(YACK_BSD)
#include <unistd.h>
#endif

using namespace yack;

YACK_UTEST(socket)
{
    network::verbose = true;
    network &nw = network::instance();
    std::cerr << "on " << nw.hostname << std::endl;

    net::socket_type s4 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
#if defined(YACK_BSD)
    shutdown(s4,SHUT_RD);
    shutdown(s4,SHUT_WR);
    shutdown(s4,SHUT_RDWR);
    close(s4);
#endif

#if defined(YACK_WIN)
    shutdown(s4, SD_BOTH);
    shutdown(s4, SD_SEND);
    shutdown(s4, SD_RECEIVE);
    closesocket(s4);
#endif

}
YACK_UDONE()

