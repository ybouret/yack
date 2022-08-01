
#include "yack/net/bsd.hpp"
#include "yack/lockable.hpp"


namespace yack
{
    namespace net
    {
        socket_type bsd:: tcp_client(socket_type s, sockaddr &sa, const sa_length_t sz)
        {
            YACK_GIANT_LOCK();
            assert(invalid_socket!=s);

#if defined(YACK_BSD)
        TRY_CONNECT:
            if(0!=connect(s,&sa,sz))
            {
                const int err = errno;
                switch(err)
                {
                    case EINPROGRESS:
                        goto TRY_CONNECT;

                    default:
                        release(s);
                        throw exception(err,"::connect");
                }
            }
#endif

#if defined(YACK_WIN)
            if(  SOCKET_ERROR == connect(s,&sa,sz) )
            {
                release(s);
                throw exception( WSAGetLastError(), "::connect");
            }
#endif

            return s;
        }
        
    }

}
