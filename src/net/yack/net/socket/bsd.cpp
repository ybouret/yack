
#include "yack/network.hpp"
#include "yack/net/bsd.hpp"

namespace yack
{

    net::socket_type network:: open(const net::ip_version level, const net::ip_protocol proto) const
    {
        YACK_LOCK(access);
        YACK_GIANT_LOCK();
        YACK_NET_PRINTLN(call_sign << ".open<" << ip_version(level) << "," << ip_protocol(proto) << ">");
        typedef net::bsd hub;
        return hub::reusable( hub::acquire(level,proto) );
    }

}
