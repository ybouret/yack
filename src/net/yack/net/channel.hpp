
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


            //! recv interface
            virtual size_t recv(void *,const size_t, const int) const = 0;
            
        protected:
            //! setup with server's address
            explicit channel() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(channel);
        };
    }
}

#endif

