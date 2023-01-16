
#include "yack/chem/reactor/balance/frontiers.hpp"

namespace yack {

    namespace chemical
    {

        frontiers:: ~frontiers() throw() {}

        frontiers:: frontiers(const size_t m) :
        base_type(m) {}

        void frontiers:: operator()(const double   xi,
                                    const species &sp,
                                    const sp_fund &F)
        {
            
        }

        
    }

}

