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

            const char    * ip_version_text(const ip_version) const noexcept;    //!< v4|v6
            const char    * ip_protocol_text(const ip_protocol) const noexcept;  //!< tcp|udp

            //__________________________________________________________________
            //
            // name resolution
            //__________________________________________________________________

            //! name resolution
            socket_address resolve(const string    &hostName,
                                   const uint16_t   hostPort,
                                   const ip_version version) const;

            //! name resolution
            socket_address resolve(const char      *hostName,
                                   const uint16_t   hostPort,
                                   const ip_version version) const;

            //! 'name:port'
            socket_address resolve(const string &fullName, const ip_version version) const;

            //! wrapper
            socket_address resolve(const char *fullName, const ip_version version) const;


            //__________________________________________________________________
            //
            // creating sockets
            //__________________________________________________________________
            socket_type    open(const ip_version, const ip_protocol) const;                    //!< low-level open a reusable socke
            socket_type    open_tcp(const ip_version) const;                                   //!< low-level open tcp
            socket_type    open_udp(const ip_version) const;                                   //!< low-level open udp
            void           connect(socket_type,const socket_address &) const;                  //!< wrapper to bsd::_connect
            void           bind(socket_type, const socket_address &)   const;                  //!< wrapper to bsd::_bind
            void           listen(socket_type, const socket_address &, const unsigned) const;  //!< wrapper to bsd::_listen
            socket_type    accept(socket_type s, socket_address &) const;                      //!< wrapper to accept


            static socket_address retrieve(const sockaddr &, const sa_length_t);                //!< build socket address from raw data






            //______________________________________________________________________
            //
            // members
            //______________________________________________________________________
            const string hostname; //!< hostname

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
            friend class singleton<plexus>;
            friend class tcp_socket;
            friend class tcp_client_;
            friend class tcp_server_;
            friend class socket;
            

            virtual ~plexus() noexcept;
            explicit plexus();


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


