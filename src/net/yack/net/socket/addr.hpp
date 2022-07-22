//! \file

#ifndef YACK_NET_SOCKET_ADDR_INCLUDED
#define YACK_NET_SOCKET_ADDR_INCLUDED 1

#include "yack/net/socket/address-metrics.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace net
    {


        class socket_addr
        {
        public:
            virtual ~socket_addr() throw();
            virtual uint16_t         family() const throw() = 0;
            virtual const char      *className() const throw() = 0;

            sockaddr        &addr;
            const size_t     size;
            net16_t         &port;

        protected:
            explicit socket_addr(sockaddr &ar, const size_t sz, net16_t &pr) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(socket_addr);
        };

      
        
    }

}

#endif
