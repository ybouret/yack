

#include "yack/network.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(resolve)
{
    network::verbose = true;
    network &nw = network::instance();

    for(int i=1;i<argc;++i)
    {
        const string              hostName = argv[i];
        try {
            const net::socket_address ipv4     = nw.resolve(hostName,net::v4,0);
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
            const net::socket_address ipv6     = nw.resolve(hostName,net::v6,0);
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

