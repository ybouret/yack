//! \file

#ifndef YACK_NET_PLEXUS_INCLUDED
#define YACK_NET_PLEXUS_INCLUDED 1

#include "yack/net/socket/address.hpp"
#include "yack/net/initializer.hpp"
#include "yack/singleton.hpp"
#include "yack/string.hpp"

namespace yack
{

    namespace net
    {
        class tcp_client;

        //______________________________________________________________________
        //
        //
        //! plexus
        //
        //______________________________________________________________________
        class plexus : private initializer, public singleton<plexus>
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

            //! wrapper
            socket_address resolve(const char *fullName, const ip_version version) const;


            //__________________________________________________________________
            //
            // creating sockets
            //__________________________________________________________________



            //______________________________________________________________________
            //
            // members
            //______________________________________________________________________
            const string hostname; //!< hostname

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
            friend class singleton<plexus>;
            friend class tcp_client;

            virtual ~plexus() throw();
            explicit plexus();

            //! low-level open a reusable socket
            socket_type   open(const net::ip_version, const net::ip_protocol) const;

            //! create a tcp client socket
            socket_type    tcp_client_socket(const socket_address &ip) const;

            //! create a tcp client socket
            template <typename HOSTNAME> inline
            socket_type    tcp_client_socket(const HOSTNAME  &hostName,
                                             const ip_version version,
                                             const uint16_t   port) const
            {
                const socket_address ip = resolve(hostName,version,port);
                return tcp_client_socket(ip);
            }

            //! create a tcp client socket
            template <typename FULLNAME> inline
            socket_type    tcp_client_socket(const FULLNAME  &fullName,
                                             const ip_version version) const
            {
                const socket_address ip = resolve(fullName,version);
                return tcp_client_socket(ip);
            }


        };


    }

}


//! macro to be used with network verbosity
#define YACK_NET_PRINTLN(MSG) do {     \
if(yack::net::plexus::verbose) {       \
YACK_LOCK(yack::net::plexus::access);  \
std::cerr << MSG << std::endl;         \
} } while(false);

//! macro to be used with network verbosity
#define YACK_NET_PRINT(MSG) do {        \
if(yack::net::plexus::verbose) {        \
YACK_LOCK(yack::net::plexus::access);   \
std::cerr << MSG;                       \
} } while(false);


#endif


