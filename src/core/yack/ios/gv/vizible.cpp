

#include "yack/ios/gv/vizible.hpp"
#include "yack/ios/encoder.hpp"
#include "yack/ios/ascii/encoded.hpp"
#include "yack/type/cstring.h"
#include <cstring>
#include <cstdlib>

namespace yack
{

    namespace ios
    {
        vizible:: ~vizible() throw() {}

        vizible:: vizible() throw() {}

        ostream & vizible:: uuid(ostream &os, const void *addr)
        {
            (void) ios::encoder::addr2hexa(os,addr);
            return os;
        }

        ostream & vizible:: end(ostream &os)
        {
            os << ';' << '\n';
            return os;
        }

        ostream & vizible:: text(ostream &fp, const char c)
        {
            fp << ascii::encoded[ uint8_t(c) ];
            return fp;
        }

        ostream & vizible:: text(ostream &fp, const char *msg)
        {
            if(msg)
            {
                while(*msg) text(fp,*(msg++));
            }
            return fp;
        }

        ostream & vizible:: arrow(ostream &os, const void *source, const void *target)
        {
            uuid(os,source) << " -> ";
            uuid(os,target);
            return os;
        }

        ostream & vizible:: digraph_init(ostream &os, const char *name)
        {
            os << "digraph " << name << '{' << '\n';
            return os;
        }

        void vizible:: digraph_quit(ostream &os)
        {
            os << '}' << '\n';
        }

        ostream & vizible:: tag(ostream &os) const
        {
            return uuid(os,this);
        }

        ostream & vizible:: link(ostream &os, const vizible *target) const
        {
            return arrow(os,this,target);
        }

        void vizible:: render(const char *filename)
        {
            static const char prolog[] = "dot -Tpng ";
            assert(filename);
            char cmd[1024];
            memset(cmd,0,sizeof(cmd));
            yack_cstring_msgcat(cmd,sizeof(cmd),prolog);

        }
    }

}

