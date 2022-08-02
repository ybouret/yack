
#include "yack/net/bsd.hpp"
#include "yack/lockable.hpp"


namespace yack
{
    namespace net
    {

        void bsd:: tcp_bind(const socket_type &s, sockaddr &sa, const sa_length_t sz)
        {
            YACK_GIANT_LOCK();
            assert(invalid_socket!=s);

#if defined(YACK_BSD)
        TRY_BIND:
            if(0!=bind(s,&sa,sz))
            {
                const int err = errno;
                switch(err)
                {
                    case EINPROGRESS:
                        goto TRY_BIND;

                    default:
                        throw exception(err,"::bind");
                }
            }
#endif

#if defined(YACK_WIN)
            if( SOCKET_ERROR == bind(s,&sa,sz) )
            {
                throw exception( WSAGetLastError(), "::bind");
            }
#endif

        }
    }

}
