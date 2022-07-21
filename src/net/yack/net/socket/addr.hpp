
//! \file

#ifndef YACK_NET_SOCKET_ADDR_INCLUDED
#define YACK_NET_SOCKET_ADDR_INCLUDED 1

#include "yack/net/socket/address-metrics.hpp"

namespace yack
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! default constructor
        //
        //______________________________________________________________________

#define YACK_NET_SOCKET_ADDR_CTOR() \
impl(),                                             \
addr( out_of_reach::access<inet_addr_type,sock_addr_type>(impl,metrics::inet_offset) ), \
port( out_of_reach::access<net16_t,       sock_addr_type>(impl,metrics::port_offset) )

        //______________________________________________________________________
        //
        //
        //! parametrized socket address
        //
        //______________________________________________________________________
        template <const ip_version ipv>
        class socket_addr : public object
        {
        public:
            //__________________________________________________________________
            //
            // parameters
            //__________________________________________________________________
            static const ip_version                   version = ipv; //!< alias
            typedef socket_address_metrics<version>   metrics;       //!< alias

            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef typename metrics::sock_addr_type  sock_addr_type; //!< alias
            typedef typename metrics::inet_addr_type  inet_addr_type; //!< alias

            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const size_t                       sock_addr_size = metrics::sock_addr_size; //!< alias
            static const size_t                       inet_addr_size = metrics::inet_addr_size; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! default constructor
            inline socket_addr() throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                metrics::initialize(impl);
            }

            //! construct with a named address and and optional port
            inline socket_addr(const inet_address_name id, const net16_t user_port=0) throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                metrics::initialize(impl);
                switch(id)
                {
                    case inet_address_any:      out_of_reach::copy(addr,metrics::any);      break;
                    case inet_address_none:     out_of_reach::copy(addr,metrics::none);     break;
                    case inet_address_loopback: out_of_reach::copy(addr,metrics::loopback); break;
                }
                (void) endian::BEaddr(&addr,inet_addr_size);
                port = YACK_NBO(user_port);
            }

            //! cleanup
            inline virtual ~socket_addr() throw()
            {
                metrics::initialize(impl);
            }

            //! copy
            inline socket_addr(const socket_addr &other) throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                memcpy(&impl,&other.impl,sock_addr_size);
            }

            //! assign
            inline socket_addr & operator=( const socket_addr &other ) throw()
            {
                memmove(&impl,&other.impl,sock_addr_size);
                return *this;
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! access implementation
            inline const sock_addr_type & sa() const throw() { return impl; }

            //! output
            friend std::ostream & operator <<( std::ostream &os, const socket_addr &self)
            {
                os << metrics::to_string(self.impl);
                if(self.port)
                {
                    os << '@' << int(YACK_NBO(self.port));
                }
                return os;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
        private:
            sock_addr_type     impl; //!< implementation

        public:
            inet_addr_type    &addr; //!< reference to internal address (nbo)
            net16_t           &port; //!< reference to internal port    (nbo)
        };

        
    }

}

#endif
