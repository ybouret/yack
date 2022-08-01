
#include "yack/net/plexus.hpp"
#include "yack/net/bsd.hpp"

namespace yack
{
    namespace net
    {

        socket_type plexus:: open(const net::ip_version level, const net::ip_protocol proto) const
        {
            YACK_LOCK(access);
            YACK_NET_PRINTLN(call_sign << ".open<" << ip_version(level) << "," << ip_protocol(proto) << ">");
            typedef net::bsd hub;
            return hub::reusable( hub::acquire(level,proto) );
        }

        socket_type plexus:: tcp_client_socket(const socket_address &ip) const
        {
            YACK_LOCK(access);
            return net::bsd::tcp_client(open(ip.version(),net::tcp),ip->addr,ip->size);
        }
    }
}
