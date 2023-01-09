
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
        all(),
        act(lib.head()),
        M(lib.size()),
        N(eqs.size()),

        Nu(N,N>0?M:0),

        libLock(lib_),
        eqsLock(eqs_)
        {
            const xmlog xml("reactor",std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"initialize");

            std::cerr << *act << std::endl;

        }

    }
}
