

#include "yack/network.hpp"
#include "yack/utest/run.hpp"
#include <cstddef>

using namespace yack;

namespace
{

}


YACK_UTEST(addr)
{
    sockaddr_in  a4;
    sockaddr_in6 a6;
    memset(&a4,0,sizeof(a4));
    memset(&a6,0,sizeof(a6));

    YACK_SIZEOF(sockaddr_in);
    YACK_SIZEOF(sockaddr_in6);



    const unsigned sin4_port_offset = offsetof(sockaddr_in,sin_port);
    const unsigned sin6_port_offset = offsetof(sockaddr_in6,sin6_port);
    const unsigned sin4_addr_offset = offsetof(sockaddr_in,sin_addr);
    const unsigned sin6_addr_offset = offsetof(sockaddr_in6,sin6_addr);

    std::cerr << "sin4_port_offset=" << sin4_port_offset << std::endl;
    std::cerr << "sin4_addr_offset=" << sin4_addr_offset << std::endl;

    std::cerr << "sin6_port_offset=" << sin6_port_offset << std::endl;
    std::cerr << "sin6_addr_offset=" << sin6_addr_offset << std::endl;




}
YACK_UDONE()

