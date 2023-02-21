//! \file

#ifndef YACK_NET_TYPES_INCLUDED
#define YACK_NET_TYPES_INCLUDED 1

#include "yack/type/uint128.hpp"
#include "yack/system/endian.hpp"
#include "yack/type/ints.hpp"

//! type to Network Byte Order
#define YACK_NBO(X) ( yack::endian::swap_be(X) )

//------------------------------------------------------------------------------
//
//
// system specific includes
//
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
// BSD-type system
//
//------------------------------------------------------------------------------
#if defined(YACK_BSD)
#   if defined(__OpenBSD__)
#      include <sys/types.h>
#   endif
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <errno.h>
#endif

//------------------------------------------------------------------------------
//
// WIN-type system
//
//------------------------------------------------------------------------------
#if defined(YACK_WIN)
#   include <winsock2.h>
#   include <ws2tcpip.h>

#        if    defined(__DMC__)
typedef struct in6_addr {
    union {
        UCHAR       Byte[16];
        USHORT      Word[8];
    } u;
} IN6_ADDR;

struct sockaddr_in6 {
    short   sin6_family;
    u_short sin6_port;
    u_long  sin6_flowinfo;
    struct  in6_addr sin6_addr;
    u_long  sin6_scope_id;
};
#        endif // __DMC__

#    if defined(_MSC_VER) || defined(__DMC__)
#        pragma comment(lib,"ws2_32.lib")
#    endif

#endif // defined(YACK_WIN)

//------------------------------------------------------------------------------
//
//
// remaining types
//
//
//------------------------------------------------------------------------------

#include "yack/exception.hpp"


namespace yack
{


    namespace net
    {
#if defined(YACK_BSD)
        typedef int error_code; //!< from errno
        //! value of the last error code
#define YACK_NET_LAST_ERROR() (errno)
#endif

#if defined(YACK_WIN)
        typedef int error_code;
        //! value of the last error code
#define YACK_NET_LAST_ERROR() (::WSAGetLastError())
#endif

        //______________________________________________________________________
        //
        //
        //! dedicated exception class
        //
        //______________________________________________________________________
        class exception : public yack::exception
        {
        public:
            //! setup
            explicit exception(const error_code err,const char *fmt,...) noexcept YACK_PRINTF_CHECK(3,4);
            virtual ~exception() noexcept;           //!< default destructor
            exception( const exception & ) noexcept; //!< copy constructor

            virtual const char *what() const noexcept; //!< internal what_
            error_code          code() const noexcept; //!< internal code_

        private:
            const error_code code_;
            char             what_[256-sizeof(error_code)];
            YACK_DISABLE_ASSIGN(exception);
        };

    }

    namespace net
    {
        //! version[4|6]
        enum ip_version
        {
            v4, //!< for IPv4
            v6  //!< for IPv6
        };

        //! protocol[UDP|TCP]
        enum ip_protocol
        {
            tcp, //!< mark TCP
            udp  //!< mark UDP
        };
        

#if defined(YACK_WIN)                   //--------------------------------------
        typedef SOCKET     socket_type; //!< win32 sockets
        typedef int        sa_length_t; //!< for sizeof(sockaddr)
#endif                                  //|
#if defined(YACK_BSD)                   //|
        typedef int        socket_type; //!< bsd sockets
        typedef socklen_t  sa_length_t; //!< for sizeof(sockaddr)
#endif                                  //|-------------------------------------

        extern const socket_type                        invalid_socket; //!< opaque invalid socket value
        typedef unsigned_int<sizeof(socket_type)>::type socket_id_t;    //!< alias for uuid
    }

}


namespace yack
{
    typedef uint8_t    net8_t;     //!< alias
    typedef uint16_t   net16_t;    //!< alias
    typedef uint32_t   net32_t;    //!< alias
    typedef uint64_t   net64_t;    //!< alias
    typedef uint128_t  net128_t;   //!< alias
}

namespace yack
{
    namespace net
    {
        //______________________________________________________________________
        //
        //! inet aliases
        //______________________________________________________________________
        enum inet_address_name
        {
            inet_address_any,      //!< any
            inet_address_none,     //!< invalid
            inet_address_loopback  //!< loopback
        };
    }

}

#endif

