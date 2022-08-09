
//! \file

#ifndef YACK_NET_CHANNEL_INCLUDED
#define YACK_NET_CHANNEL_INCLUDED 1

#include "yack/net/socket/address.hpp"

namespace yack
{
    namespace net
    {
        //! channel for operations
        class channel
        {
        public:
            virtual ~channel() throw(); //!< cleanup

            const socket_address host; //!< a.k.a server address

        protected:
            //! setup with server's address
            explicit channel(const socket_address &srv) throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(channel);
        };
    }
}

#endif

