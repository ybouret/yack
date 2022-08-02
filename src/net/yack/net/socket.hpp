//! \file

#ifndef YACK_NET_SOCKET_INCLUDED
#define YACK_NET_SOCKET_INCLUDED 1

#include "yack/net/socket/address.hpp"

namespace yack
{
    namespace net
    {

        //______________________________________________________________________
        //
        //
        //! base class for socket++
        //
        //______________________________________________________________________
        class socket : public gateway<const socket_address>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~socket() throw();            //!< close socket

        protected:
            explicit socket(socket_type, const socket_address &) throw(); //!< setup from user's sock

        protected:
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const socket_type    sock; //!< descriptor
            const socket_address self; //!< socket address
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(socket);
            virtual const_type  &bulk() const throw();
        };
    }
}

#endif

