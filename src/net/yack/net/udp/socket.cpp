#include "yack/net/udp/socket.hpp"

namespace yack
{

    namespace net
    {

        udp_socket:: ~udp_socket() throw() {}

        udp_socket:: udp_socket(const plexus &network, const ip_version version) :
        socket( socket_address(version), network.open_udp(version) )
        {
        }



        void udp_socket:: sendTo(const socket_address &dest,
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
            const int ret = sendto(sock,(const char *)msg,len,flags, &(dest->addr), dest->size);
            if(SOCKET_ERROR==ret) throw exception( WSAGetLastError(),"sendto");
            if( static_cast<size_t>(ret) != len ) throw yack::exception("truncated sendto");
#endif

        }

        size_t udp_socket:: recvFrom(socket_address &src,
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
            const int    res = recvfrom(sock,(char*)msg,len,flags,&sa,&sz);
            if(SOCKET_ERROR==res) throw exception( WSAGetLastError(),"recvfrom");
#endif

            src = plexus::retrieve(sa,sz);
            return static_cast<size_t>(res);
        }
        

    }

}
 
