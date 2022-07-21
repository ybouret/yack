
#include "yack/net/socket/address-metrics.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace net
    {

        void socket_address_ops:: add_dec_to(string &ans, const uint8_t d)
        {
            ans += vformat("%d",int(d));
        }

    }

}

