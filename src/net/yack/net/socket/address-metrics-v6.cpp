
#include "yack/net/socket/address-metrics-v6.hpp"
#include "yack/string.hpp"
#include "yack/type/out-of-reach.hpp"
#include <cstring>

namespace yack
{
    namespace net
    {
        void socket_address_metrics<v6>:: initialize(sock_addr_type &sa) throw()
        {
            memset(&sa,0,sock_addr_size);
            sa.sin6_family = AF_INET6;
        }


        string socket_address_metrics<v6>:: to_string(const sock_addr_type &sa)
        {
            string  ans(64,as_capacity);
            {
                const uint8_t *h = coerce_cast<uint8_t>( &sa.sin6_addr );
                socket_address_ops::add_dec_to(ans,h[15]);
                for(int i=14;i>=0;--i)
                {
                    ans += ':';
                    socket_address_ops::add_dec_to(ans,h[i]);
                }

            }
            return ans;
        }

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

        const char socket_address_metrics<v6>::className[] = "IPv6";

    }

}


