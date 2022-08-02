
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

        void plexus:: connect(socket_type sock,const socket_address &self) const
        {
            YACK_NET_PRINTLN('[' << call_sign << ".connect<" << self << ">" << ']');
            bsd::tcp_connect(sock,self->addr, self->size);
        }


        void plexus:: bind(socket_type sock,const socket_address &self) const
        {
            YACK_NET_PRINTLN('[' << call_sign << ".bind<" << self << ">" << ']');
            bsd::tcp_bind(sock,self->addr, self->size);
        }

        void plexus:: listen(socket_type sock, const socket_address &self, const unsigned pending) const
        {
            YACK_NET_PRINTLN('[' << call_sign << ".listen<" << self << "," << pending << ">" << ']');
            bsd::tcp_listen(sock,pending);
        }


        socket_address plexus:: retrieve(const sockaddr &sa, const sa_length_t sz)
        {

            switch(sz)
            {
                case sizeof(sockaddr_in): {
                    socket_address res(v4);
                    memcpy( &(res->addr), &sa, sizeof(sockaddr_in));
                    return res;
                } break;

                case sizeof(sockaddr_in6): {
                    socket_address res(v6);
                    memcpy( &(res->addr), &sa, sizeof(sockaddr_in6));
                    return res;
                } break;

                default:
                    break;
            }

            throw yack::exception("invalid sizeof(sockaddr)=%u", unsigned(sz) );


        }

        socket_type plexus:: tcp_accept(socket_type srv, socket_address &cln) const
        {
            YACK_GIANT_LOCK();
            assert(invalid_socket!=srv);
            sockaddr_in6 sa6;
            sockaddr    &sa = coerce_to<sockaddr>(sa6);
            sa_length_t  sz = sizeof(sa);

#if defined(YACK_BSD)
        TRY_CONNECT:
            socket_type sock = accept(srv,&sa,&sz);
            if(sock<0)
            {
                const int err = errno;
                switch(err)
                {
                    case EINPROGRESS:
                        goto TRY_CONNECT;

                    default:
                        throw exception(err,"::accept");
                }
            }
#endif

#if defined(YACK_WIN)
            socket_type sock = accept(srv,&sa,&sz);
            if(invalid_socket==sock)
            {
                throw exception(WSAGetLastError(),"::accept");
            }
#endif
            cln = retrieve(sa,sz);
            YACK_NET_PRINTLN( '[' << call_sign << ".accept <" << cln << ">" << ']');
            return sock;
        }


        socket_type  plexus:: open_udp(const ip_version level) const
        {
            return open(level,udp);
        }


    }
}
