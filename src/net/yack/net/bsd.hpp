
//! \file

#ifndef YACK_NET_BSD_INCLUDED
#define YACK_NET_BSD_INCLUDED 1

#include "yack/net/types.hpp"

namespace yack
{
    namespace net
    {
        //______________________________________________________________________
        //
        //! shutdow operations
        //______________________________________________________________________
        enum sd_how
        {
            sd_send, //!< shutdown send
            sd_recv, //!< shutdown recv
            sd_both  //!< shutdown both
        };
        
        //______________________________________________________________________
        //
        //
        //! BSD sockets API
        //
        //______________________________________________________________________
        struct bsd
        {

            static socket_type acquire(const ip_version, const ip_protocol);  //!< open socket
            static void        release(socket_type &)                throw(); //!< close socket
            static void        closure(socket_type &, const sd_how)  throw(); //!< perform shutdown

            //__________________________________________________________________
            //
            // getting options
            //__________________________________________________________________
            static void        getopt(const socket_type &s,
                                      const int          level,
                                      const int          optName,
                                      void              *optVal,
                                      unsigned          *optLen);

            template <typename T> static inline
            T getopt(const socket_type &s, const int level, const int optName)
            {
                T        res(0);
                unsigned len = sizeof(res);
                getopt(s,level,optName,&res,&len);
                return res;
            }

            //__________________________________________________________________
            //
            // setting options
            //__________________________________________________________________
            static void        setopt(socket_type       &s,
                                      const int          level,
                                      const int          optName,
                                      const void        *optVal,
                                      const unsigned     optLen);

            template <typename T> static inline
            void setopt(socket_type       &s,
                        const int          level,
                        const int          optName,
                        const T            optVal)
            {
                const unsigned optLen = sizeof(optVal);
                setopt(s,level,optName,&optVal,optLen);
            }

        };
    }
}
#endif

