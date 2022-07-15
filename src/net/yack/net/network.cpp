
#include "yack/net/network.hpp"

namespace yack
{
    network:: ~network() throw()
    {
    }

    network:: network() : singleton<network>()
    {
    }

    const char   network:: call_sign[] = "network";

}


