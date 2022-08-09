
//! \file

#ifndef YACK_NET_CHANNEL_INCLUDED
#define YACK_NET_CHANNEL_INCLUDED 1

#include "yack/net/socket/address.hpp"

namespace yack
{
    namespace net
    {
        class channel
        {
        public:
            virtual ~channel() throw();

            virtual const socket_address & host() const throw() = 0;

        protected:
            explicit channel() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(channel);
        };
    }
}

#endif

