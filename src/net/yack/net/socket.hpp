//! \file

#ifndef YACK_NET_SOCKET_INCLUDED
#define YACK_NET_SOCKET_INCLUDED 1

#include "yack/net/socket/address.hpp"
#include "yack/associative/be-key.hpp"

namespace yack
{
    namespace net
    {

        //! UUID for socket
        typedef be_key<const socket_type> socket_key;

        class plexus;
        
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
            explicit socket(const socket_address &, socket_type) throw(); //!< setup from user's sock
            explicit socket(const plexus &, const socket_type);           //!< setup from tcp server

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const socket_address  self; //!< socket address
            const socket_type     sock; //!< descriptor
            const socket_key      skey; //!< uuid for sock


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(socket);
            virtual const_type  &bulk() const throw();

        public:
            const socket_key & key() const throw(); //!< get UUID
        };
    }
}

#endif

