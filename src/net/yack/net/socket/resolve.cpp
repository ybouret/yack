#include "yack/network.hpp"

#if defined(YACK_BSD)
#include <netdb.h>
#endif

#if defined(YACK_WIN)

#    if defined(__DMC__)
extern "C" {

    struct addrinfo {
        int              ai_flags;
        int              ai_family;
        int              ai_socktype;
        int              ai_protocol;
        size_t           ai_addrlen;
        char *           ai_canonname;
        struct sockaddr *ai_addr;
        struct addrinfo *ai_next;
    };

    int WSAAPI getaddrinfo(
                           const char *nodename,
                           const char *servname,
                           const struct addrinfo *fmt,
                           struct addrinfo **res
                           );

    void WSAAPI freeaddrinfo( struct addrinfo *ai );


}
#    endif // __DMC__

#if defined(__GNUC__)
extern "C"
{

    int WSAAPI getaddrinfo(const char            *nodename,
                           const char            *servname,
                           const struct addrinfo *fmt,
                           struct addrinfo       **res
                           );

    void WSAAPI freeaddrinfo( struct addrinfo *ai );
}
#endif

#endif // YACK_WIN

namespace yack
{



    net::socket_address network:: resolve(const string &hostName, const net::ip_version version) const
    {
        YACK_NET_PRINTLN(call_sign << ".resolve<" << hostName << ">");
        
        addrinfo fmt;
        memset( &fmt, 0, sizeof(fmt) );
        fmt.ai_family = net::socket_address::family(version);

        addrinfo *ai0 = NULL;
        int       err = ::getaddrinfo( hostName(), NULL, &fmt, &ai0);
        if( err )
        {
#           if defined(Y_WIN)
            throw win32::exception( err, "::getaddrinfo(%s,%s)" , *name, ip.className() );
#           endif

#           if defined(Y_BSD)
            throw imported::exception( gai_strerror(err), "::getaddrinfo(%s,%s)" , *name, ip.className() );
#           endif
        }

        // TODO: sanity ?
        assert( ai0             != NULL        );
        assert( ai0->ai_addr    != NULL        );
        assert( ai0->ai_addrlen == ip.length() );

        const net16_t p = ip.port; //-- save port
        memcpy( ip.rw(), ai0->ai_addr, ip.length() );
        ip.port = p;               //-- restore port

        ::freeaddrinfo(ai0);

    }

}
