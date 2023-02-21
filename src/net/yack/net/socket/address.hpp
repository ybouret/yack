

//! \file

#ifndef YACK_NET_SOCKET_ADDRESS_INCLUDED
#define YACK_NET_SOCKET_ADDRESS_INCLUDED 1


#include "yack/net/socket/ipv4.hpp"
#include "yack/net/socket/ipv6.hpp"
#include "yack/memory/workplace.hpp"
#include "yack/memory/buffer/rw.hpp"
#include "yack/type/gateway.hpp"

namespace yack
{
    namespace net
    {

        //______________________________________________________________________
        //
        //
        //! variant address to hold IPv4/IPv6
        //
        //______________________________________________________________________
        class socket_address : public gateway<socket_addr>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup with version and optional inet_address_name and port
            socket_address(const ip_version        user_ip_v,
                           const inet_address_name user_name = inet_address_none,
                           const uint16_t          user_port = 0) noexcept;

            //! full copy
            socket_address(const socket_address &other) noexcept;

            //! copy/swap
            socket_address & operator=(const socket_address &other) noexcept;

            //! cleanup
            virtual ~socket_address() noexcept;



            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void        xch(socket_address &) noexcept; //!< no-throw swap
            ip_version  version()       const noexcept; //!< version

            //! display
            friend std::ostream & operator<<(std::ostream &, const socket_address &);

        private:
            ip_version              which;
            void                   *entry;
            memory::workplace<IPv6> where;
            void clear() noexcept;
            virtual const_inward & bulk()    const noexcept;

        };
    }
}


#endif
