
#include "yack/chem/eqs/constant.hpp"

namespace yack
{
    namespace chemical
    {
        const_equilibrium:: ~const_equilibrium() throw() {}
        
        
        double const_equilibrium:: getK(double) const
        {
            return K_;
        }
        
        
    }
    
}
