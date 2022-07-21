

#include "yack/network.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/hexa.hpp"
#include <cstddef>
#include <cstring>

using namespace yack;

namespace yack
{
    namespace net
    {
        template <const  ip_version>
        struct socket_address_metrics;


        static inline void add_dec_to(string &ans, const uint8_t b)
        {
            ans += vformat("%d",int(b));
        }

        enum inet_address_name
        {
            inet_address_any,
            inet_address_none,
            inet_address_loopback
        };

        template <>
        struct socket_address_metrics<v4>
        {

            typedef sockaddr_in      sock_addr_type;
            typedef net32_t          inet_addr_type;
            static  const  size_t    sock_addr_size = sizeof(sock_addr_type);
            static  const size_t     inet_addr_size = sizeof(inet_addr_type);
            static  const  ptrdiff_t inet_offset    = offsetof(sockaddr_in,sin_addr);
            static  const  ptrdiff_t port_offset    = offsetof(sockaddr_in,sin_port);

            static const inet_addr_type any      = INADDR_ANY;
            static const inet_addr_type none     = INADDR_NONE;
            static const inet_addr_type loopback = INADDR_LOOPBACK;

            static inline void initialize(sock_addr_type &sa) throw()
            {
                memset(&sa,0,sock_addr_size);
                sa.sin_family = AF_INET;
            }


            static inline string to_string(const sock_addr_type &sa)
            {
                string  ans(16,as_capacity);
                {
                    net32_t dw = (sa.sin_addr.s_addr);
                    add_dec_to(ans,dw);
                    for(size_t i=0;i<3;++i)
                    {
                        ans += '.'; add_dec_to(ans,dw>>=8);
                    }
                }
                return ans;
            }


        };

        const socket_address_metrics<v4>::inet_addr_type socket_address_metrics<v4>::any;
        const socket_address_metrics<v4>::inet_addr_type socket_address_metrics<v4>::none;
        const socket_address_metrics<v4>::inet_addr_type socket_address_metrics<v4>::loopback;


        template <>
        struct socket_address_metrics<v6>
        {
            typedef sockaddr_in6     sock_addr_type;
            typedef net128_t         inet_addr_type;
            static  const size_t     sock_addr_size = sizeof(sock_addr_type);
            static  const size_t     inet_addr_size = sizeof(inet_addr_type);
            static  const ptrdiff_t  inet_offset    = offsetof(sockaddr_in6,sin6_addr);
            static  const ptrdiff_t  port_offset    = offsetof(sockaddr_in6,sin6_port);

            static const inet_addr_type any;
            static const inet_addr_type none;
            static const inet_addr_type loopback;

            static inline void initialize(sock_addr_type &sa) throw()
            {
                memset(&sa,0,sock_addr_size);
                sa.sin6_family = AF_INET6;
            }

            static inline string to_string(const sock_addr_type &sa)
            {
                string  ans(64,as_capacity);
                {
                    const uint8_t *h = coerce_cast<uint8_t>( &sa.sin6_addr );
                    add_dec_to(ans,h[15]);
                    for(int i=15;i>=0;--i)
                    {
                        ans += ':';
                        add_dec_to(ans,h[i]);
                    }

                }
                return ans;
            }

        };

        static const uint8_t data_any[1][1][16]      = IN6ADDR_ANY_INIT;
        static const uint8_t data_loopback[1][1][16] = IN6ADDR_LOOPBACK_INIT;
        static const uint8_t data_none[16]           =
        {
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
        };

        const socket_address_metrics<v6>::inet_addr_type  socket_address_metrics<v6>::any      = data_any[0][0];
        const socket_address_metrics<v6>::inet_addr_type  socket_address_metrics<v6>::loopback = data_loopback[0][0];
        const socket_address_metrics<v6>::inet_addr_type  socket_address_metrics<v6>::none     = data_none;



#define YACK_NET_SOCKET_ADDR_CTOR() sa(),                                             \
addr( out_of_reach::access<inet_addr_type,sock_addr_type>(sa,metrics::inet_offset) ), \
port( out_of_reach::access<net16_t,       sock_addr_type>(sa,metrics::port_offset) )

        template <const ip_version ipv>
        class socket_addr : public object
        {
        public:
            typedef socket_address_metrics<ipv>       metrics;
            typedef typename metrics::sock_addr_type  sock_addr_type;
            typedef typename metrics::inet_addr_type  inet_addr_type;
            static const size_t                       sock_addr_size = metrics::sock_addr_size;
            static const size_t                       inet_addr_size = metrics::inet_addr_size;

            socket_addr() throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                metrics::initialize(sa);
            }

            socket_addr(const inet_address_name id) throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                metrics::initialize(sa);
                switch(id)
                {
                    case inet_address_any :     memcpy(&addr,&metrics::any,     inet_addr_size); break;
                    case inet_address_none:     memcpy(&addr,&metrics::none,    inet_addr_size); break;
                    case inet_address_loopback: memcpy(&addr,&metrics::loopback,inet_addr_size); break;
                }
                (void) endian::BEaddr(&addr,inet_addr_size);
            }

            virtual ~socket_addr() throw()
            {
                metrics::initialize(sa);
            }

            socket_addr(const socket_addr &other) throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                memcpy(&sa,&other.sa,sock_addr_size);
            }

            socket_addr & operator=( const socket_addr &other ) throw()
            {
                memmove(&sa,&other.sa,sock_addr_size);
                return *this;
            }

        public:
            sock_addr_type     sa;
            inet_addr_type    &addr;
            net16_t           &port;
        };




    }

}


YACK_UTEST(addr)
{
    sockaddr_in  a4;
    sockaddr_in6 a6;
    memset(&a4,0,sizeof(a4));
    memset(&a6,0,sizeof(a6));

    YACK_SIZEOF(sockaddr_in);
    YACK_SIZEOF(sockaddr_in6);



    const unsigned sin4_port_offset = offsetof(sockaddr_in,sin_port);
    const unsigned sin6_port_offset = offsetof(sockaddr_in6,sin6_port);
    const unsigned sin4_addr_offset = offsetof(sockaddr_in,sin_addr);
    const unsigned sin6_addr_offset = offsetof(sockaddr_in6,sin6_addr);

    std::cerr << "sin4_port_offset=" << sin4_port_offset << std::endl;
    std::cerr << "sin4_addr_offset=" << sin4_addr_offset << std::endl;

    std::cerr << "sin6_port_offset=" << sin6_port_offset << std::endl;
    std::cerr << "sin6_addr_offset=" << sin6_addr_offset << std::endl;


    net::socket_addr<net::v4> ipv4; std::cerr << "ipv4@" << ipv4.addr << std::endl;
    net::socket_addr<net::v6> ipv6; std::cerr << "ipv6@" << ipv6.addr << std::endl;

    YACK_SIZEOF(net::socket_addr<net::v4>);
    YACK_SIZEOF(net::socket_addr<net::v6>);

    ipv4 = net::inet_address_any;      std::cerr << net::socket_address_metrics<net::v4>::to_string(ipv4.sa) << std::endl;
    ipv4 = net::inet_address_none;     std::cerr << net::socket_address_metrics<net::v4>::to_string(ipv4.sa) << std::endl;
    ipv4 = net::inet_address_loopback; std::cerr << net::socket_address_metrics<net::v4>::to_string(ipv4.sa) << std::endl;

    ipv6 = net::inet_address_any;      std::cerr << net::socket_address_metrics<net::v6>::to_string(ipv6.sa) << std::endl;
    ipv6 = net::inet_address_none;     std::cerr << net::socket_address_metrics<net::v6>::to_string(ipv6.sa) << std::endl;
    ipv6 = net::inet_address_loopback; std::cerr << net::socket_address_metrics<net::v6>::to_string(ipv6.sa) << std::endl;


}
YACK_UDONE()

