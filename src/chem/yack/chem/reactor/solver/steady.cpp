
#include "yack/chem/reactor/solver/steady.hpp"

namespace yack {

    namespace chemical {


        steady:: ~steady() throw() {}

        steady:: steady(const reactor    &_,
                        writable<double> &K_,
                        const double     t0) :
        cs(_),
        K(K_),
        Cs(cs.L,cs.L>0?cs.M:0)
        {
            cs.computeK(K,t0);
        }
        
        

    }

}

