#include "yack/net/tcp/client.hpp"

namespace yack
{
    namespace net
    {
        tcp_client_:: ~tcp_client_() throw()
        {
        }

        tcp_client_:: tcp_client_(const plexus &network, const socket_address client_addr) :
        tcp_socket(network,client_addr)
        {
            network.connect(sock,self);
        }



    }

    namespace net
    {
        tcp_client:: ~tcp_client() throw()
        {
        }

        tcp_client:: tcp_client(const plexus &network, const socket_address client_addr) :
        tcp_client_(network,client_addr)
        {
        }
    }




}

#include "yack/net/tcp/server.hpp"
#include "yack/type/ints.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace net
    {

        tcp_client_:: tcp_client_(const plexus &network, const tcp_server &srv) :
        tcp_socket(network,srv.sock)
        {

        }

        tcp_client:: tcp_client(const plexus &network, const tcp_server &srv) :
        tcp_client_(network,srv)
        {

        }

        size_t tcp_client:: send(const void  *buffer,
                                 const size_t buflen,
                                 const int    flags)
        {
            YACK_GIANT_LOCK();
            assert(yack_good(buffer,buflen));
            static const size_t MAXBUFLEN = integral_for<int>::maximum;
            const size_t        to_send   = min_of(MAXBUFLEN,buflen);
#if defined(YACK_BSD)
        TRY_SEND:
            const ssize_t sent = ::send(sock,buffer,to_send,flags);
            if(sent<0)
            {
                const int err = errno;
                switch(err)
                {
                    case EINTR:
                        goto TRY_SEND;
                        
                    default:
                        throw exception(err,"send(%lu)", static_cast<unsigned long>(to_send));
                }
            }
#endif

#if defined(YACK_WIN)
            const int sent = ::send(sock,static_cast<const char*>(buffer),to_send,flags);
            if(SOCKET_ERROR == sent)
            {
                const int err = WSAGetLastError();
                throw exception(err,"send(%lu)", static_cast<unsigned long>(to_send));

            }
#endif

            return static_cast<size_t>(sent);

        }

    }

}
