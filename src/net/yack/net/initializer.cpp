#include "yack/net/initializer.hpp"
#include "yack/net/types.hpp"
#include "yack/lockable.hpp"
#include "yack/string.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/buffers.hpp"

#include <cstring>

#if defined(YACK_BSD)
#include <csignal>
#include <unistd.h>
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

        string initializer:: get_host_name() const
        {
            YACK_GIANT_LOCK();
            size_t len = 256;

#if defined(YACK_BSD)
            for(;;)
            {
                memory::buffer_of<char,memory::pooled> blk(len);
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
                memory::buffer_of<char,memory::pooled> blk(len);
                if( ::gethostname( *blk, int(blk.measure()) ) == SOCKET_ERROR ) {
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
        
    }
}

