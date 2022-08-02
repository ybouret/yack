#include "yack/net/udp/socket.hpp"
 
namespace yack
{

    namespace net
    {

        udp_socket_:: ~udp_socket_() throw() {}

        udp_socket_:: udp_socket_(const plexus &network, const socket_address user_addr) :
        socket(user_addr,network.open_udp(user_addr.version()) )
        {
        }



        void udp_socket_:: sendTo(const socket_address &dest,
                                  const void           *msg,
                                  const size_t          len,
                                  const int             flags)
        {
            assert(yack_good(msg,len));
            YACK_GIANT_LOCK();

#if defined(YACK_BSD)
            const ssize_t ret = sendto(sock,msg,len,flags, &(dest->addr), dest->size);
            if(ret<0) throw exception(errno,"sendto");
            if( static_cast<size_t>(ret) != len ) throw yack::exception("truncated sendto");
#endif

#if defined(YACK_WIN)
            const int ret = sendto(sock,msg,len,flags, &(dest->addr), dest->size);
            if(SOCKET_ERROR==ret) throw exception( WSAGetLastError(),"sendto");
            if( static_cast<size_t>(ret) != len ) throw yack::exception("truncated sendto");
#endif

        }

        size_t udp_socket_:: recvFrom(socket_address &src,
                                      void           *msg,
                                      const size_t    len,
                                      const int       flags)
        {
            assert(yack_good(msg,len));
            YACK_GIANT_LOCK();
            sockaddr_in6 sa6;
            sockaddr    &sa = coerce_to<sockaddr>(sa6);
            sa_length_t  sz = sizeof(sockaddr_in6);

#if defined(YACK_BSD)
            const ssize_t res = recvfrom(sock,msg,len,flags, &sa, &sz);
            if(res<0) throw exception(errno,"recvfrom");
#endif

#if defined(YACK_WIN)
            const int    res = recfrom(sock,msg,len,flags,&sa,&sz);
            if(SOCKET_ERROR==res) throw exception( WSAGetLastError(),"recvfrom");
#endif

            return static_cast<size_t>(res);
        }


    }

}
namespace yack
{

    namespace net
    {

        udp_socket:: ~udp_socket() throw() {}

        udp_socket:: udp_socket(const plexus &network, const socket_address user_addr) :
        udp_socket_(network,user_addr)
        {
        }

        udp_socket:: udp_socket(const plexus &network, const uint16_t port, const ip_version version) :
        udp_socket_(network, socket_address(version,inet_address_any,port) )
        {

        }

    }

}
