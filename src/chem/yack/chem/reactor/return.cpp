
#include "yack/chem/reactor.hpp"


namespace yack
{

    namespace chemical
    {

        bool reactor:: returnSolved(writable<double> &C0)
        {
            working.transfer(C0,Corg);
            if(verbose)
            {
                corelib(std::cerr << "Cend=", "", C0);
            }
            return true;
        }

        
    }

}

