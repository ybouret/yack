#include "yack/net/plexus.hpp"

#include <cstring>



namespace yack
{

    namespace net
    {
        bool plexus::verbose = false;

        plexus:: ~plexus() throw()
        {
            YACK_NET_PRINTLN( '[' << call_sign << ".cleanup " << hostname << ']');
        }




        plexus:: plexus() :  initializer(), singleton<plexus>(),
        hostname( get_host_name() )
        {
            YACK_NET_PRINTLN( '[' << call_sign << ".startup " << hostname << ']');
        }

        const char   plexus:: call_sign[] = "plexus";

        const uint16_t plexus:: reserved_port   = IPPORT_RESERVED;
#       if defined(YACK_WIN) || defined(__FreeBSD__)
        const uint16_t plexus:: first_user_port = IPPORT_RESERVED;
#        else
        const uint16_t plexus:: first_user_port = IPPORT_USERRESERVED;
#        endif

        const uint16_t plexus:: user_port_width = (plexus::final_user_port-plexus::first_user_port)+1;


        ip_version plexus:: ip_version_from(const char   *text) const
        {
            static const char fn[] = ".ip_version";
            if(!text) throw yack::exception("%s%s(NULL)",call_sign,fn);
            if(!strcmp(text,"v4")) return net::v4;
            if(!strcmp(text,"v6")) return net::v6;
            throw yack::exception("%s%s(invalid '%s')",call_sign,fn,text);
        }

        net::ip_version plexus:: ip_version_from(const string &text) const
        {
            return ip_version_from( text() );
        }

        const char    * plexus:: ip_version_text(const  ip_version pf) const throw()
        {
            switch(pf)
            {
                case net::v4: return "v4";
                case net::v6: return "v6";
            }
            return yack_unknown;
        }

        const char * plexus:: ip_protocol_text(const net::ip_protocol proto) const throw()
        {
            switch(proto)
            {
                case net::tcp: return "tcp";
                case net::udp: return "udp";
            }
            return yack_unknown;
        }

    }

}


