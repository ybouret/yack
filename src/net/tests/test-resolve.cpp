

#include "yack/net/plexus.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(resolve)
{
    net::plexus::verbose = true;
    net::plexus &network = net::plexus::instance();

    for(int i=1;i<argc;++i)
    {
        const string              hostName = argv[i];
        try {
            const net::socket_address ipv4     = network.resolve(hostName,0,net::v4);
            std::cerr << ipv4 << std::endl;
        }
        catch(const exception &e)
        {
            e.show(std::cerr);
        }
        catch(...)
        {
            throw;
        }

        try {
            const net::socket_address ipv6     = network.resolve(hostName,0,net::v6);
            std::cerr << ipv6 << std::endl;
        }
        catch(const exception &e)
        {
            e.show(std::cerr);
        }
        catch(...)
        {
            throw;
        }


    }



}
YACK_UDONE()

