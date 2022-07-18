
#include "yack/net/network.hpp"
#include "yack/system/exception.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/buffers.hpp"
#include <cstring>

#if defined(YACK_BSD)
#include <csignal>
#include <unistd.h>
#include <cerrno>
#endif


namespace yack
{

    namespace net
    {
        namespace
        {

#if defined(YACK_WIN)
            static WSADATA wsa;
#endif

        }

        initializer:: ~initializer() throw()
        {

#if defined(YACK_WIN)
            ::WSACleanup();
#endif
        }

        initializer:: initializer()
        {
            YACK_GIANT_LOCK();

#if defined(YACK_WIN)
            memset(&wsa, 0, sizeof(WSADATA) );
            if( :: WSAStartup( MAKEWORD(2,2), &wsa ) !=  0 )
            {
                throw win32::exception( ::WSAGetLastError(), "::WSAStartup" );
            }
#endif

#if defined(YACK_BSD)
            signal( SIGPIPE, SIG_IGN );
#endif
        }

    }



    bool network::verbose = false;
    
    network:: ~network() throw()
    {
        YACK_NET_PRINTLN( '[' << call_sign << ".cleanup" << ']');
    }

    static inline string   get_host_name()  
    {
        YACK_GIANT_LOCK();
        size_t len = 256;

#if defined(YACK_BSD)
        for(;;)
        {
            memory::buffer_of<char,memory::pooled> blk(len);
            //zblock<char,memory::pooled> blk( len );
            if( ::gethostname( *blk, blk.measure() ) != 0)
            {
                len *= 2;
                continue;
            }
            return string( *blk );
        }
#endif

#if defined(YACK_WIN)
        for(;;) {
            zblock<char,memory::pooled> blk( len );
            if( ::gethostname( *blk, int(blk.length()) ) == SOCKET_ERROR ) {
                const DWORD err = ::WSAGetLastError();
                if( err != WSAEFAULT )
                    throw   win32::exception( err, "::gethostname" );
                len *= 2;
                continue;
            }
            return string( *blk );
        }
#endif

    }


    network:: network() : net::initializer(), singleton<network>(),
    hostname()
    {
        YACK_NET_PRINTLN( '[' << call_sign << ".startup" << ']');


    }

    const char   network:: call_sign[] = "network";

    const uint16_t network:: reserved_port   = IPPORT_RESERVED;
#       if defined(YACK_WIN) || defined(__FreeBSD__)
    const uint16_t network:: first_user_port = IPPORT_RESERVED;
#        else
    const uint16_t network:: first_user_port = IPPORT_USERRESERVED;
#        endif

    const uint16_t network:: user_port_width = (network::final_user_port-network::first_user_port)+1;


    net::ip_version network:: ip_version(const char   *text) const
    {
        static const char fn[] = ".ip_version";
        if(!text) throw yack::exception("%s%s(NULL)",call_sign,fn);
        if(!strcmp(text,"v4")) return net::v4;
        if(!strcmp(text,"v6")) return net::v6;
        throw yack::exception("%s%s(invalid '%s')",call_sign,fn,text);
    }

    net::ip_version network:: ip_version(const string &text) const
    {
        return ip_version( text() );
    }


}


