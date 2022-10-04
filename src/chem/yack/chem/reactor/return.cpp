
#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack
{

    namespace chemical
    {

        bool reactor:: returnSolved(writable<double> &C0, const xmlog &xml)
        {
            working.transfer(C0,Corg);
            if(verbose)
            {
                corelib(*xml << "Cend=", "", C0);
            }
            return true;
        }

        
    }

}

