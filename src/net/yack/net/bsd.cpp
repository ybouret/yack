
#include "yack/net/bsd.hpp"
#include "yack/lockable.hpp"

#if defined(YACK_BSD)
#include <unistd.h>
#include <fcntl.h>
#endif

#include <iostream>

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
            (void) close(s);
            s = invalid_socket;
#endif

#if defined(YACK_WIN)
            (void) closesocket(s);
            s = invalid_socket;
#endif

            assert(invalid_socket == s);

        }

#if defined(YACK_BSD)
#define YACK_NET_SD_SEND SHUT_WR
#define YACK_NET_SD_RECV SHUT_RD
#define YACK_NET_SD_BOTH SHUT_RDWR
#endif

#if defined(YACK_WIN)
#define YACK_NET_SD_SEND SD_SEND
#define YACK_NET_SD_RECV SD_RECEIVE
#define YACK_NET_SD_BOTH SD_BOTH
#endif

        void bsd:: closure(socket_type &s, const sd_how how) throw()
        {
            switch(how)
            {
                case sd_send: (void) shutdown(s,YACK_NET_SD_SEND); break;
                case sd_recv: (void) shutdown(s,YACK_NET_SD_RECV); break;
                case sd_both: (void) shutdown(s,YACK_NET_SD_BOTH); break;
            }
        }


        void bsd:: getopt(const socket_type &s,
                          const int          level,
                          const int          optName,
                          void              *optVal,
                          unsigned          *optLen)
        {
            YACK_GIANT_LOCK();

            assert(invalid_socket != s);
            assert(NULL!=optVal);
            assert(NULL!=optLen);

#if defined(YACK_BSD)
            socklen_t tmpLen = *optLen;
            if( getsockopt(s,level,optName,optVal,&tmpLen) < 0 )
            {
                throw exception( errno, "getsockopt" );
            }
            *optLen = tmpLen;
#endif

#if defined(YACK_WIN)
            int tmpLen = *optLen;
            if( SOCKET_ERROR == getsockopt(s,level,optName,(char*)optVal,&tmpLen) )
            {
                throw exception( ::WSAGetLastError(), "getsockopt" );
            }
            *optLen = tmpLen;
#endif

        }


        void bsd:: setopt(socket_type       &s,
                          const int          level,
                          const int          optName,
                          const void        *optVal,
                          const unsigned     optLen)
        {
            YACK_GIANT_LOCK();

            assert(invalid_socket != s);
            assert(NULL!=optVal);

#if defined(YACK_BSD)
            if( setsockopt(s,level,optName,optVal,optLen) < 0 )
            {
                throw exception( errno, "setsockopt" );
            }
#endif

#if defined(YACK_WIN)
            if( setsockopt(s,level,optName,(const char *)optVal, static_cast<int>(optLen) ) < 0 )
            {
                throw exception( WSAGetLastError(), "setsockopt" );
            }
#endif

        }

        void bsd:: set_blocking(socket_type &s, const bool flag)
        {
            YACK_GIANT_LOCK();
            assert(invalid_socket != s);

#if defined(YACK_BSD)
            int flags = fcntl (s, F_GETFL, 0);
            if(flags<0) throw exception(errno,"fcntl(F_GETGL)");
            if(flag)
            {
                flags |= O_NONBLOCK;
            }
            else
            {
                flags &= ~(O_NONBLOCK);
            }
            if( fcntl(s,F_SETFL,flags) < 0 ) throw exception(errno,"fcntl(F_SETFL)");
#endif

#if defined(YACK_WIN)
            u_long iMode = flag ? 1 : 0;

            //-------------------------
            // Set the socket I/O mode: In this case FIONBIO
            // enables or disables the blocking mode for the
            // socket based on the numerical value of iMode.
            // If iMode = 0, blocking is enabled;
            // If iMode != 0, non-blocking mode is enabled.

            const int iResult = ioctlsocket(s, FIONBIO, &iMode);
            if( NO_ERROR != iResult) throw exception(WSAGetLastError(),"ioctlsocket");
#endif

        }


        socket_type bsd:: reusable(socket_type s)
        {
            assert(invalid_socket!=s);
            try        { const int reUse = 1; setopt(s,SOL_SOCKET,SO_REUSEADDR,reUse); }
            catch(...) { release(s); throw; }
            return s;
        }

    }
}
