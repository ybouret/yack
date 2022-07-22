
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
        gateway<socket_addr>(),
        which(user_ip_v),
        entry(NULL),
        where(entry)
        {
            switch(which)
            {
                case v4:  (new (entry) IPv4(user_name,user_port)) ; break;
                case v6:  (new (entry) IPv6(user_name,user_port)) ; break;
            }
        }

        socket_address:: socket_address(const socket_address &other) throw() :
        gateway<socket_addr>(),
        which(other.which),
        entry(NULL),
        where(entry)
        {
            switch(which)
            {
                case v4:   new (entry) IPv4( *static_cast<const IPv4 *>(other.entry) ); break;
                case v6:   new (entry) IPv6( *static_cast<const IPv6 *>(other.entry) ); break;
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

        socket_address:: const_type & socket_address:: bulk() const throw()
        {
            const socket_addr *myself = NULL;
            switch(which)
            {
                case v4: myself = static_cast<const IPv4 *>(entry); break;
                case v6: myself = static_cast<const IPv6 *>(entry); break;
            }
            assert(NULL!=myself);
            return *myself;
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

