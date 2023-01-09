
#include "yack/chem/reactor.hpp"

namespace yack
{
    namespace chemical
    {
        reactor:: ~reactor() throw()
        {
        }


        reactor:: reactor(library     &lib_,
                          equilibria  &eqs_,
                          const double t0) :
        lib(lib_),
        eqs(eqs_),
        M(lib.size()),
        N(eqs.size()),

        Nu(N,N>0?M:0),

        libLock(lib_),
        eqsLock(eqs_)
        {

        }

    }
}
