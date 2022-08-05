


#include "yack/net/udp/server.hpp"
#include "yack/net/plexus.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;

YACK_UTEST(udp_server)
{
    net::plexus::verbose = true;
    net::plexus &network = net::plexus::instance();
    std::cerr << program << " on " << network.hostname << std::endl;

    if( argc > 1)
    {
        const uint16_t        srvport  = ios::ascii::convert::to<uint16_t>(argv[1],"port");
        const net::ip_version version  = (argc>2) ? network.ip_version_from(argv[2]) : net::v4;
        net::udp_server       server(network,version,srvport);

#if 0
        net::socket_address   peer(net::v4);
        char                  msg[256];
        memset(msg,0,sizeof(msg));
        const size_t nr = server.recvFrom(peer,msg,sizeof(msg)-1);
        std::cerr << "From " << peer << " : #" << nr << "='" << msg << "'" << std::endl;
#endif

    }

}
YACK_UDONE()

