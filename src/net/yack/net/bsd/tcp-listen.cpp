
#include "yack/net/bsd.hpp"
#include "yack/lockable.hpp"


namespace yack
{
    namespace net
    {

        void bsd:: _listen(const socket_type &s,
                           const unsigned     pending)
        {
            YACK_GIANT_LOCK();
            assert(pending>0);

#if defined(YACK_BSD)
            if( listen(s,pending) < 0 ) throw exception(errno, "::listen");
#endif

#if defined(YACK_WIN)
            if( SOCKET_ERROR == listen(s,pending) ) throw exception( WSAGetLastError(), "::listen");
#endif

        }
    }

}
