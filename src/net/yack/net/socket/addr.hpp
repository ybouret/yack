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

        //______________________________________________________________________
        //
        //
        //! socket address with mapping of members
        //
        //______________________________________________________________________
        class socket_addr
        {
        public:
            virtual            ~socket_addr()     noexcept;      //!< cleanup
            virtual uint16_t    family()    const noexcept = 0;  //!< AF_INET[6]
            virtual const char *className() const noexcept = 0;  //!< "IPv[4|6]"
            
            sockaddr         &addr; //!< reference for BSD
            const sa_length_t size; //!< effective length of addr
            net16_t          &port; //!< reference to internal port

        protected:
            //! setup with references
            explicit socket_addr(sockaddr &ar, const sa_length_t sz, net16_t &pr) noexcept;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(socket_addr);
        };

      
        
    }

}

#endif
