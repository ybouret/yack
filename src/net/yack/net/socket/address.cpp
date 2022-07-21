
#include "yack/net/socket/address.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{
    namespace net
    {
        socket_address::  ~socket_address() throw() { clear(); }

        socket_address:: socket_address(const ip_version        user_ip_v,
                                        const inet_address_name user_name,
                                        const uint16_t          user_port) throw() :
        which(user_ip_v),
        entry(NULL),
        where(entry)
        {
            switch(which)
            {
                case v4: (new (entry) IPv4(user_name,user_port)) ; break;
                case v6: (new (entry) IPv6(user_name,user_port)) ; break;
            }
        }

        socket_address:: socket_address(const socket_address &other) throw() :
        which(other.which),
        entry(NULL),
        where(entry)
        {
            switch(which)
            {
                case v4: new (entry) IPv4( *static_cast<const IPv4 *>(other.entry) ); break;
                case v6: new (entry) IPv6( *static_cast<const IPv6 *>(other.entry) ); break;
            }
        }

        void socket_address:: xch(socket_address &other) throw()
        {
            cswap(which,other.which);
            where.swap_with(other.where);
        }

        socket_address & socket_address:: operator=(const socket_address &other) throw()
        {
            { socket_address tmp(other); xch(tmp); }
            return *this;
        }

        ip_version  socket_address:: version() const throw() { return which; }

        uint16_t socket_address:: port() const throw() {
            switch(which)
            {
                case v4: return YACK_NBO(static_cast<const IPv4 *>(entry)->port);
                case v6: return YACK_NBO(static_cast<const IPv6 *>(entry)->port);
            }
            return 0;
        }

        uint16_t socket_address:: family() const throw()
        {
            return family(which);
        }

        uint16_t socket_address:: family(const ip_version v) throw()
        {
            switch (v) {
                case v4: return AF_INET;
                case v6: return AF_INET6;
            }
            return 0;
        }


        void socket_address:: port(const uint16_t user_port) throw()
        {
            switch(which)
            {
                case v4: static_cast<const IPv4 *>(entry)->port = YACK_NBO(user_port); break;
                case v6: static_cast<const IPv6 *>(entry)->port = YACK_NBO(user_port); break;
            }
        }

        std::ostream & operator<<(std::ostream &os, const socket_address &self)
        {
            switch(self.which)
            {
                case v4: os << *static_cast<const IPv4 *>(self.entry); break;
                case v6: os << *static_cast<const IPv6 *>(self.entry); break;
            }
            return os;
        }

        void socket_address:: clear() throw()
        {
            switch(which)
            {
                case v4: destruct(static_cast<const IPv4 *>(entry)); break;;
                case v6: destruct(static_cast<const IPv6 *>(entry)); break;;
            }
            where.eradicate();
        }
    }

}

