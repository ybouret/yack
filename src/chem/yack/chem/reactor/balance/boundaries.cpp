
#include "yack/chem/reactor/balance/boundaries.hpp"

namespace yack {

    namespace chemical
    {

        boundaries:: boundaries(const size_t m) :
        limiting(m),
        amending(m)
        {
        }

        boundaries:: ~boundaries() throw()
        {
        }

        

    }

}

