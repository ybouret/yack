#include "yack/chem/component.hpp"

namespace yack
{
    namespace chemical
    {
        component:: ~component() throw()
        {
        }
        
        component:: component(const species &sr,
                              const int      cf) throw() :
        
        sp(sr),
        nu(cf)
        {
            assert(nu!=0);
        }
        
    }
}
        
