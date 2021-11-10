

#include "yack/ios/gv/vizible.hpp"
#include "yack/ios/encoder.hpp"

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

        void vizible:: digraph_quit(ostream &os)
        {
            os << '}' << '\n';
        }


    }

}

