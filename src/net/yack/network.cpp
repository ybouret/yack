#include "yack/network.hpp"

#include <cstring>



namespace yack
{

    
    bool network::verbose = false;
    
    network:: ~network() throw()
    {
        YACK_NET_PRINTLN( '[' << call_sign << ".cleanup " << hostname << ']');
    }




    network:: network() : net::initializer(), singleton<network>(),
    hostname( get_host_name() )
    {
        YACK_NET_PRINTLN( '[' << call_sign << ".startup " << hostname << ']');
    }

    const char   network:: call_sign[] = "network";

    const uint16_t network:: reserved_port   = IPPORT_RESERVED;
#       if defined(YACK_WIN) || defined(__FreeBSD__)
    const uint16_t network:: first_user_port = IPPORT_RESERVED;
#        else
    const uint16_t network:: first_user_port = IPPORT_USERRESERVED;
#        endif

    const uint16_t network:: user_port_width = (network::final_user_port-network::first_user_port)+1;


    net::ip_version network:: ip_version(const char   *text) const
    {
        static const char fn[] = ".ip_version";
        if(!text) throw yack::exception("%s%s(NULL)",call_sign,fn);
        if(!strcmp(text,"v4")) return net::v4;
        if(!strcmp(text,"v6")) return net::v6;
        throw yack::exception("%s%s(invalid '%s')",call_sign,fn,text);
    }

    net::ip_version network:: ip_version(const string &text) const
    {
        return ip_version( text() );
    }

    const char    * network:: ip_version(const net::ip_version pf) const throw()
    {
        switch(pf)
        {
            case net::v4: return "v4";
            case net::v6: return "v6";
        }
        return yack_unknown;
    }

    const char * network:: ip_protocol(const net::ip_protocol proto) const throw()
    {
        switch(proto)
        {
            case net::tcp: return "tcp";
            case net::udp: return "udp";
        }
        return yack_unknown;
    }


}


