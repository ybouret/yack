

#include "yack/net/tcp/server.hpp"
#include "yack/net/tcp/client.hpp"
#include "yack/net/plexus.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;

YACK_UTEST(tcp_server)
{
    net::plexus::verbose = true;
    net::plexus &network = net::plexus::instance();
    std::cerr << program << " on " << network.hostname << std::endl;

    if( argc > 1)
    {
        const uint16_t        srvport  = ios::ascii::convert::to<uint16_t>(argv[1],"port");
        const net::ip_version version  = (argc>2) ? network.ip_version_from(argv[2]) : net::v4;
        net::tcp_server srv(network,version,srvport,1);
        char buffer[256];
        while(true)
        {
            net::tcp_client cln(network,srv);
            while(true)
            {
                memset(buffer,0,sizeof(buffer));
                const size_t nr = cln.recv(buffer,sizeof(buffer),0);
                if(nr<=0)
                {
                    break;
                }
                std::cerr.write(buffer,nr);
                cln.send_all(buffer,nr,0);
            }



        }

    }

}
YACK_UDONE()

