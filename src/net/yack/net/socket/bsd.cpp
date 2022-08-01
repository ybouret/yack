
#include "yack/net/plexus.hpp"
#include "yack/net/bsd.hpp"

namespace yack
{
    namespace net
    {

        socket_type plexus:: open(const net::ip_version level, const net::ip_protocol proto) const
        {
            YACK_LOCK(access);
            YACK_NET_PRINTLN('[' << call_sign << ".open<" << ip_protocol_text(proto) << "," << ip_version_text(level)  << ">" << ']');
            typedef net::bsd hub;
            return hub::reusable( hub::acquire(level,proto) );
        }

        socket_type plexus:: open_tcp(const ip_version level) const
        {
            return open(level,tcp);
        }

        void plexus:: tcp_client(socket_type sock,const socket_address &self) const
        {
            YACK_NET_PRINTLN('[' << call_sign << ".connect<" << self << ">" << ']');
            bsd::tcp_client(sock,self->addr, self->size);
        }


        socket_type  plexus:: open_udp(const ip_version level) const
        {
            return open(level,udp);
        }


        
    }
}
