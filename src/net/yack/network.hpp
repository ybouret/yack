//! \file

#ifndef YACK_NETWORK_INCLUDED
#define YACK_NETWORK_INCLUDED 1

#include "yack/net/socket/address.hpp"
#include "yack/net/initializer.hpp"
#include "yack/singleton.hpp"
#include "yack/string.hpp"

namespace yack
{

    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! network
        //
        //______________________________________________________________________
        class network : private net::initializer, public singleton<network>
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static bool           verbose;                 //!< mostly to debug
            static const uint16_t reserved_port;           //!< port < reserved_port: for system
            static const uint16_t first_user_port;         //!< port >= first_user_port: for user
            static const uint16_t final_user_port = 65535; //!< for information
            static const uint16_t user_port_width;         //!< final_user_port-first_user_port+1;

            //__________________________________________________________________
            //
            // singleton
            //__________________________________________________________________
            static const at_exit::longevity life_time = 3000; //!< for singleton
            static const char               call_sign[];      //!< for singleton

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            ip_version ip_version_from(const char   *text) const; //!< [v4|v6]
            ip_version ip_version_from(const string &text) const; //!< [v4|v6]

            const char    * ip_version_text(const ip_version) const throw();    //!< v4|v6
            const char    * ip_protocol_text(const ip_protocol) const throw();  //!< tcp|udp

            //__________________________________________________________________
            //
            // name resolution
            //__________________________________________________________________

            //! name resolution
            socket_address resolve(const string    &hostName,
                                   const ip_version version,
                                   const uint16_t   port) const;

            //! name resolution
            socket_address resolve(const char      *hostName,
                                   const ip_version version,
                                   const uint16_t   port) const;

            //! 'name:port'
            socket_address resolve(const string &fullName, const ip_version version) const;

            //__________________________________________________________________
            //
            // creating sockets
            //__________________________________________________________________

            //! create a tcp client socket
            net::socket_type    tcp_client_socket(const net::socket_address &ip) const;

            //______________________________________________________________________
            //
            // members
            //______________________________________________________________________
            const string hostname; //!< hostname

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(network);
            friend class singleton<network>;

            virtual ~network() throw();
            explicit network();

            //! low-level open a reusable socket
            net::socket_type   open(const net::ip_version, const net::ip_protocol) const;

        };
    }

}


//! macro to be used with network verbosity
#define YACK_NET_PRINTLN(MSG) do {       \
if(yack::net::network::verbose) {        \
YACK_LOCK(yack::net::network::access);   \
std::cerr << MSG << std::endl;           \
} } while(false);

//! macro to be used with network verbosity
#define YACK_NET_PRINT(MSG) do {         \
if(yack::net::network::verbose) {        \
YACK_LOCK(yack::net::network::access);   \
std::cerr << MSG;                        \
} } while(false);


#endif


