

//! \file

#ifndef YACK_NET_SOCKET_ADDRESS_INCLUDED
#define YACK_NET_SOCKET_ADDRESS_INCLUDED 1


#include "yack/net/socket/ipv4.hpp"
#include "yack/net/socket/ipv6.hpp"
#include "yack/memory/workplace.hpp"

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
        class socket_address
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup with version and optional inet_address_name and port
            socket_address(const ip_version        user_ip_v,
                           const inet_address_name user_name = inet_address_none,
                           const uint16_t          user_port = 0) throw();

            //! full copy
            socket_address(const socket_address &other) throw();

            //! copy/swap
            socket_address & operator=(const socket_address &other) throw();

            //! cleanup
            virtual ~socket_address() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static uint16_t family(const ip_version) throw();

            void       xch(socket_address &) throw(); //!< no-throw swap
            ip_version version()       const throw(); //!< current version
            uint16_t   port()          const throw(); //!< read port
            void       port(const uint16_t)  throw(); //!< set port
            uint16_t   family()        const throw(); //!< return family

            //! display
            friend std::ostream & operator<<(std::ostream &, const socket_address &);

        private:
            ip_version              which;
            void                   *entry;
            memory::workplace<IPv6> where;
            void clear() throw();
        };
    }
}


#endif
