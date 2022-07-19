
#include "yack/net/bsd.hpp"
#include "yack/lockable.hpp"

#if defined(YACK_BSD)
#include <unistd.h>
#endif

namespace yack
{
    namespace net
    {
        socket_type bsd:: acquire(const net::ip_version  domain,
                                  const net::ip_protocol protocol)
        {
            YACK_GIANT_LOCK();


            int dom = 0;
            switch(domain)
            {
                case v4: dom = PF_INET;    break;
                case v6: dom = PF_INET6;   break;
            }

            int stype = 0;
            int proto = 0;
            switch(protocol)
            {
                case tcp: stype = SOCK_STREAM; proto = IPPROTO_TCP; break;
                case udp: stype = SOCK_DGRAM;  proto = IPPROTO_UDP; break;
            }


            const socket_type s = socket(dom,stype,proto);
            if(invalid_socket==s)
                throw exception( YACK_NET_LAST_ERROR(), "bsd::acquire");
            return s;
        }

        void bsd:: release(socket_type &s) throw()
        {
            assert(invalid_socket != s);

#if defined(YACK_BSD)
            (void) close(s);       s = invalid_socket;
#endif

#if defined(YACK_WIN)
            (void) closesocket(s); s =  invalid_socket;
#endif

            assert(net::invalid_socket == s);

        }

#if defined(YACK_BSD)
#define YACK_NET_SD_SEND SHUT_WR
#define YACK_NET_SD_RECV SHUT_RD
#define YACK_NET_SD_BOTH SHUT_RDWR
#endif

#if defined(YACK_WIN)
#define YACK_NET_SD_SEND SD_SEND
#define YACK_NET_SD_RECV SD_RECV
#define YACK_NET_SD_BOTH SD_BOTH
#endif

        void bsd:: perform(socket_type &s, const sd_how how) throw()
        {
            switch(how)
            {
                case sd_send: (void) shutdown(s,YACK_NET_SD_SEND); break;
                case sd_recv: (void) shutdown(s,YACK_NET_SD_RECV); break;
                case sd_both: (void) shutdown(s,YACK_NET_SD_BOTH); break;
            }
        }

    }
}
