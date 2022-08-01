//! \file

#ifndef YACK_NET_SOCKET_INCLUDED
#define YACK_NET_SOCKET_INCLUDED 1

#include "yack/net/types.hpp"

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
        class socket
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~socket() throw();            //!< close socket

        protected:
            explicit socket(socket_type) throw(); //!< setup from user's sock

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            socket_type sock; //!< descriptor

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(socket);
        };
    }
}

#endif

