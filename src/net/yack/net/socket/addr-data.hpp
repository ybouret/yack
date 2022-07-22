
//! \file

#ifndef YACK_NET_SOCKET_DATA_INCLUDED
#define YACK_NET_SOCKET_DATA_INCLUDED 1

#include "yack/net/socket/addr.hpp"
#include "yack/type/out-of-reach.hpp"


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

#define YACK_NET_SOCKET_ADDR_CTOR()                 \
socket_addr( _ar_(), sock_addr_size, _pr_() ),      \
impl(),                                             \
addr(   out_of_reach::access<inet_addr_type,sock_addr_type>(impl,metrics::addr_offset) )

        //______________________________________________________________________
        //
        //
        //! parametrized socket address
        //
        //______________________________________________________________________
        template <const ip_version ipv>
        class socket_addr_data : public socket_addr
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
            inline socket_addr_data() throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                metrics::initialize(impl);
            }

            //! construct with a named address and and optional port
            inline socket_addr_data(const inet_address_name id, const net16_t user_port=0) throw() : YACK_NET_SOCKET_ADDR_CTOR()
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
            inline virtual ~socket_addr_data() throw()
            {
                metrics::initialize(impl);
            }

            //! copy
            inline socket_addr_data(const socket_addr_data &other) throw() : YACK_NET_SOCKET_ADDR_CTOR()
            {
                memcpy(&impl,&other.impl,sock_addr_size);
            }

            //! assign
            inline socket_addr_data & operator=( const socket_addr_data &other ) throw()
            {
                memmove(&impl,&other.impl,sock_addr_size);
                return *this;
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________


            virtual uint16_t         family()    const throw() { return metrics::family;    }
            virtual const char      *className() const throw() { return metrics::className; }

            //! output
            friend std::ostream & operator <<( std::ostream &os, const socket_addr_data &self)
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

            inline net16_t  & _pr_() throw() { return out_of_reach::access<net16_t,sock_addr_type>(impl,metrics::port_offset); }
            inline sockaddr & _ar_() throw() { return coerce_to<sockaddr>(impl); }

        public:
            inet_addr_type    &addr;   //!< reference to internal address (nbo)
        };


    }

}

#endif
