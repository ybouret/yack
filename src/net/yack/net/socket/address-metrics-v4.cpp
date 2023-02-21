#include "yack/net/socket/address-metrics-v4.hpp"
#include "yack/string.hpp"
#include <cstring>

namespace yack
{
    namespace net
    {

        void  socket_address_metrics<v4>:: initialize(sock_addr_type &sa) noexcept
        {
            memset(&sa,0,sock_addr_size);
            sa.sin_family = AF_INET;
        }

        string socket_address_metrics<v4>:: to_string(const sock_addr_type &sa)
        {
            string  ans(16,as_capacity);
            {
                net32_t dw = (sa.sin_addr.s_addr);
                socket_address_ops::add_dec_to(ans,dw);
                for(size_t i=0;i<3;++i)
                {
                    ans += '.'; socket_address_ops::add_dec_to(ans,dw>>=8);
                }
            }
            return ans;
        }

        const socket_address_metrics<v4>::inet_addr_type socket_address_metrics<v4>::any;
        const socket_address_metrics<v4>::inet_addr_type socket_address_metrics<v4>::none;
        const socket_address_metrics<v4>::inet_addr_type socket_address_metrics<v4>::loopback;

        const char socket_address_metrics<v4>::className[] = "IPv4";


    }

}
