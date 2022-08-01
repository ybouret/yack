//! \file

#ifndef YACK_NETWORK_INCLUDED
#define YACK_NETWORK_INCLUDED 1

#include "yack/net/socket/address.hpp"
#include "yack/net/initializer.hpp"
#include "yack/singleton.hpp"
#include "yack/string.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! network
    //
    //__________________________________________________________________________
    class network : private net::initializer, public singleton<network>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        static bool           verbose;                 //!< mostly to debug
        static const uint16_t reserved_port;           //!< port < reserved_port: for system
        static const uint16_t first_user_port;         //!< port >= first_user_port: for user
        static const uint16_t final_user_port = 65535; //!< for information
        static const uint16_t user_port_width;         //!< final_user_port-first_user_port+1;

        //______________________________________________________________________
        //
        // singleton
        //______________________________________________________________________
        static const at_exit::longevity life_time = 3000; //!< for singleton
        static const char               call_sign[];      //!< for singleton

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        net::ip_version ip_version(const char   *text) const; //!< [v4|v6]
        net::ip_version ip_version(const string &text) const; //!< [v4|v6]

        const char    * ip_version(const net::ip_version) const throw();    //!< v4|v6
        const char    * ip_protocol(const net::ip_protocol) const throw();  //!< tcp|udp

        //! name resolution
        net::socket_address resolve(const string         &hostName,
                                    const net::ip_version version,
                                    const uint16_t        port = 0) const;

        

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
    };

}


//! macro to be used with network verbosity
#define YACK_NET_PRINTLN(MSG) do {  \
if(yack::network::verbose) {        \
YACK_LOCK(yack::network::access);   \
std::cerr << MSG << std::endl;      \
} } while(false);

//! macro to be used with network verbosity
#define YACK_NET_PRINT(MSG) do {    \
if(yack::network::verbose) {        \
YACK_LOCK(yack::network::access);   \
std::cerr << MSG;                   \
} } while(false);


#endif


